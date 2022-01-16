// function& operator=(function &&other) {
//   if (m_delete) m_delete(*this);
//   m_functor = other.m_functor;
//   m_invoke = other.m_invoke;
//   m_delete = other.m_delete;
//   m_copy = other.m_copy;
//   other.m_functor = nullptr;
//   other.m_invoke = nullptr;
//   other.m_delete = nullptr;
//   other.m_copy = nullptr;
//   return *this;
// }