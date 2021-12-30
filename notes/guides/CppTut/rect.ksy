meta:
  id: rect
  endian: be
  encoding: ascii
seq:
  - id: x_pos
    type: s4
  - id: y_pos
    type: s4
  - id: width
    type: u4
  - id: height
    type: u4
  - id: color
    type: color_type
types: 
  color_type:
    seq:
      - id: red
        type: u1
      - id: green
        type: u1
      - id: blue
        type: u1
      - id: alpha
        type: u1

