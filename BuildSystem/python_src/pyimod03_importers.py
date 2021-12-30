# Source Generated with Decompyle++
# File: ./pyimod03_importers.pyc (Python 3.8)

'''
PEP-302 and PEP-451 importers for frozen applications.
'''
import sys
import _frozen_importlib
import pyimod01_os_path as pyi_os_path
from pyimod02_archive import ArchiveReadError, ZlibArchiveReader
SYS_PREFIX = sys._MEIPASS
SYS_PREFIXLEN = len(SYS_PREFIX)
imp_new_module = type(sys)
if sys.flags.verbose:
    
    def trace(msg, *a):
        sys.stderr.write(msg % a)
        sys.stderr.write('\n')

else:
    
    def trace(msg, *a):
        pass


class FrozenPackageImporter(object):
    '''
    Wrapper class for FrozenImporter that imports one specific fullname from
    a module named by an alternate fullname. The alternate fullname is derived from the
    __path__ of the package module containing that module.

    This is called by FrozenImporter.find_module whenever a module is found as a result
    of searching module.__path__
    '''
    
    def __init__(self, importer, entry_name):
        self._entry_name = entry_name
        self._importer = importer

    
    def load_module(self, fullname):
        return self._importer.load_module(fullname, self._entry_name)



class FrozenImporter(object):
    """
    Load bytecode of Python modules from the executable created by PyInstaller.

    Python bytecode is zipped and appended to the executable.

    NOTE: PYZ format cannot be replaced by zipimport module.

    The problem is that we have no control over zipimport; for instance,
    it doesn't work if the zip file is embedded into a PKG appended
    to an executable, like we create in one-file.

    This is PEP-302 finder and loader class for the ``sys.meta_path`` hook.
    A PEP-302 finder requires method find_module() to return loader
    class with method load_module(). Both these methods are implemented
    in one class.

    This is also a PEP-451 finder and loader class for the ModuleSpec type
    import system. A PEP-451 finder requires method find_spec(), a PEP-451
    loader requires methods exec_module(), load_module(9 and (optionally)
    create_module(). All these methods are implemented in this one class.

    To use this class just call

        FrozenImporter.install()
    """
    
    def __init__(self):
        '''
        Load, unzip and initialize the Zip archive bundled with the executable.
        '''
        for pyz_filepath in sys.path:
            
            try:
                self._pyz_archive = ZlibArchiveReader(pyz_filepath)
                sys.path.remove(pyz_filepath)
                self.toc = set(self._pyz_archive.toc.keys())
                trace('# PyInstaller: FrozenImporter(%s)', pyz_filepath)
            finally:
                return None
                except IOError:
                    continue
                    continue
                    except ArchiveReadError:
                        continue
                        continue
                    
                    raise ImportError("Can't load frozen modules.")
                    return None


    
    def find_module(self, fullname, path = (None,)):
        """
        PEP-302 finder.find_module() method for the ``sys.meta_path`` hook.

        fullname     fully qualified name of the module
        path         None for a top-level module, or package.__path__
                     for submodules or subpackages.

        Return a loader object if the module was found, or None if it wasn't.
        If find_module() raises an exception, it will be propagated to the
        caller, aborting the import.
        """
        module_loader = None
        if fullname in self.toc:
            module_loader = self
            trace('import %s # PyInstaller PYZ', fullname)
        elif path is not None:
            modname = fullname.split('.')[-1]
            if not parts[0]:
                parts = parts[1:]
            parts.append(modname)
            entry_name = '.'.join(parts)
            if entry_name in self.toc:
                module_loader = FrozenPackageImporter(self, entry_name)
                trace('import %s as %s # PyInstaller PYZ (__path__ override: %s)', entry_name, fullname, p)
            
        if module_loader is None:
            trace('# %s not found in PYZ', fullname)
        return module_loader

    
    def load_module(self, fullname, entry_name = (None,)):
        '''
        PEP-302 loader.load_module() method for the ``sys.meta_path`` hook.

        Return the loaded module (instance of imp_new_module()) or raises
        an exception, preferably ImportError if an existing exception
        is not being propagated.

        When called from FrozenPackageImporter, `entry_name` is the name of the
        module as it is stored in the archive. This module will be loaded and installed
        into sys.modules using `fullname` as its name
        '''
        module = None
        if entry_name is None:
            entry_name = fullname
        
        try:
            module = sys.modules.get(fullname)
            if module is None:
                (is_pkg, bytecode) = self._pyz_archive.extract(entry_name)
                module = imp_new_module(fullname)
                module.__file__ = self.get_filename(entry_name)
                if is_pkg:
                    module.__path__ = [
                        pyi_os_path.os_path_dirname(module.__file__)]
                module.__loader__ = self
                if is_pkg:
                    module.__package__ = fullname
                else:
                    module.__package__ = fullname.rsplit('.', 1)[0]
                module.__spec__ = _frozen_importlib.ModuleSpec(entry_name, self, is_pkg, **('is_package',))
                sys.modules[fullname] = module
                exec(bytecode, module.__dict__)
                module = sys.modules[fullname]
        finally:
            pass
        except Exception:
            if fullname in sys.modules:
                sys.modules.pop(fullname)
            raise 
        

        return module

    
    def is_package(self, fullname):
        pass
    # WARNING: Decompyle incomplete

    
    def get_code(self, fullname):
        '''
        Get the code object associated with the module.

        ImportError should be raised if module not found.
        '''
        pass
    # WARNING: Decompyle incomplete

    
    def get_source(self, fullname):
        '''
        Method should return the source code for the module as a string.
        But frozen modules does not contain source code.

        Return None.
        '''
        if fullname in self.toc:
            return None
        raise None('No module named ' + fullname)

    
    def get_data(self, path):
        '''
        This returns the data as a string, or raise IOError if the "file"
        wasn\'t found. The data is always returned as if "binary" mode was used.

        This method is useful getting resources with \'pkg_resources\' that are
        bundled with Python modules in the PYZ archive.

        The \'path\' argument is a path that can be constructed by munging
        module.__file__ (or pkg.__path__ items)
        '''
        if not path.startswith(SYS_PREFIX + pyi_os_path.os_sep):
            raise AssertionError
        fullname = None[SYS_PREFIXLEN + 1:]
        if fullname in self.toc:
            return self._pyz_archive.extract(fullname)[1]
    # WARNING: Decompyle incomplete

    
    def get_filename(self, fullname):
        '''
        This method should return the value that __file__ would be set to
        if the named module was loaded. If the module is not found, then
        ImportError should be raised.
        '''
        if self.is_package(fullname):
            filename = pyi_os_path.os_path_join(pyi_os_path.os_path_join(SYS_PREFIX, fullname.replace('.', pyi_os_path.os_sep)), '__init__.pyc')
        else:
            filename = pyi_os_path.os_path_join(SYS_PREFIX, fullname.replace('.', pyi_os_path.os_sep) + '.pyc')
        return filename

    
    def find_spec(self, fullname, path, target = (None, None)):
        '''
        PEP-451 finder.find_spec() method for the ``sys.meta_path`` hook.

        fullname     fully qualified name o