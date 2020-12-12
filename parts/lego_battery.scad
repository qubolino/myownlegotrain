include <flexbatter.scad>
include <lego_basic.scad>

translate([UNIT_LENGTH*2,0,0])brick(1,2,1, false, true);
translate([UNIT_LENGTH*6,0,0])brick(1,2,1, false, true);

translate([68.0,11.5,2.2])rotate([0,0,180])flexbatter18650(n=1);