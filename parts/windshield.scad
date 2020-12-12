include <lego_basic.scad>

space = (PLAY-tolerance);

difference(){
    union(){
        // base blocks
        translate([0,8,0])brick(6,1,1, false, false);
        translate([0,0,0])brick(1,1,1, false, false);
        translate([40,0,0])brick(1,1,1, false, false);
        translate([0,0,9.6*2])brick(1,1,1, true, false);
        translate([40,0,9.6*2])brick(1,1,1, true, false);

        // play gap filling
        translate([space, UNIT_LENGTH-space, 0]) cube([UNIT_LENGTH-2*space, 2*space, PLATE_HEIGHT]);
        translate([5*UNIT_LENGTH+space, UNIT_LENGTH-space, 0]) cube([UNIT_LENGTH-2*space, 2*space, PLATE_HEIGHT]);
        
        // vertical beams
        translate([space,space,PLATE_HEIGHT]) cube([WALL_THICKNESS, WALL_THICKNESS, 6*PLATE_HEIGHT]);
        translate([6*UNIT_LENGTH-WALL_THICKNESS-space,space,PLATE_HEIGHT]) cube([WALL_THICKNESS, WALL_THICKNESS, 6*PLATE_HEIGHT]);

        
        // horizontal beams
        translate([space,UNIT_LENGTH-2*WALL_THICKNESS-space,7*PLATE_HEIGHT-ROOF_THICKNESS]) cube([6*UNIT_LENGTH-2*space, 2*WALL_THICKNESS,ROOF_THICKNESS]);
        translate([space,UNIT_LENGTH-0.5*WALL_THICKNESS-space*2,7.2*PLATE_HEIGHT-ROOF_THICKNESS]) rotate([22.5-90,0,0])cube([6*UNIT_LENGTH-2*space, 2*WALL_THICKNESS,ROOF_THICKNESS]);
        
        // slope beams
        translate([space,2*UNIT_LENGTH-WALL_THICKNESS,PLATE_HEIGHT-ROOF_THICKNESS+2*space]) rotate([22.5,0,0])cube([WALL_THICKNESS,WALL_THICKNESS,6.55*PLATE_HEIGHT]);
        translate([6*UNIT_LENGTH-WALL_THICKNESS-space,2*UNIT_LENGTH-WALL_THICKNESS,PLATE_HEIGHT-ROOF_THICKNESS+2*space]) rotate([22.5,0,0])cube([WALL_THICKNESS,WALL_THICKNESS,6.55*PLATE_HEIGHT]);    
        translate([3*UNIT_LENGTH-WALL_THICKNESS/2,2*UNIT_LENGTH-WALL_THICKNESS,PLATE_HEIGHT-ROOF_THICKNESS+2*space]) rotate([22.5,0,0])cube([WALL_THICKNESS,WALL_THICKNESS,6.55*PLATE_HEIGHT]);
    
    }
    
    // lower holes
    translate([UNIT_LENGTH-space,0,0]) cylinder(r=4,h=PLATE_HEIGHT);
    translate([5*UNIT_LENGTH+space,0,0]) cylinder(r=4,h=PLATE_HEIGHT);
    
    // play gap filling underneath removal
    translate([WALL_THICKNESS+PLAY/2-tolerance,UNIT_LENGTH-WALL_THICKNESS-space,0]) cube([UNIT_LENGTH-2*WALL_THICKNESS-PLAY+tolerance*2,4,PLATE_HEIGHT-ROOF_THICKNESS]);
    translate([5*UNIT_LENGTH+WALL_THICKNESS+PLAY/2-tolerance,UNIT_LENGTH-WALL_THICKNESS-space,0]) cube([UNIT_LENGTH-2*WALL_THICKNESS-PLAY+tolerance*2,4,PLATE_HEIGHT-ROOF_THICKNESS]);
    
    
    translate([WALL_THICKNESS+PLAY,0,6*PLATE_HEIGHT])cube([UNIT_LENGTH-WALL_THICKNESS,UNIT_LENGTH-WALL_THICKNESS,PLATE_HEIGHT-ROOF_THICKNESS]);
    translate([5*UNIT_LENGTH-space,0,6*PLATE_HEIGHT])cube([UNIT_LENGTH-WALL_THICKNESS,UNIT_LENGTH-WALL_THICKNESS,PLATE_HEIGHT-ROOF_THICKNESS]);
    
}


