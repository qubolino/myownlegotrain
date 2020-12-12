
include <lego_basic.scad>

// container front

module container(width=6, height=3, sides=true) {
    translate([0,0,PLATE_HEIGHT*(height*3-1)])
            brick(width,1,1, true, false);
    brick(width,1,1, false, true);
    translate([PLAY/2+tolerance, PLAY/2+tolerance ,PLATE_HEIGHT])
        cube([UNIT_LENGTH*width-PLAY-2*tolerance,1.5-PLAY,PLATE_HEIGHT*(height*3-1)]);
        
    if(sides) {
        translate([PLAY/2+tolerance, PLAY/2+tolerance,PLATE_HEIGHT])
            cube([1.5,UNIT_LENGTH*1-PLAY-2*tolerance,PLATE_HEIGHT*(height*3-1)]);
        translate([UNIT_LENGTH*width-1.5-PLAY/2-tolerance, PLAY/2+tolerance,PLATE_HEIGHT])
            cube([1.5,UNIT_LENGTH*1-PLAY-2*tolerance,PLATE_HEIGHT*(3*height-1)]);

    }
}


module profile(){
    translate([0,3,0])rotate([180,0,0]) polygon([[0,0],[0,1],[1,1],[3,3],[5,3],[7,1],[8,1],[8,0],[6.6,0],[4.6,2],[3.4,2],[1.4,0]]);
}

module container_v3(width=6, height=3, sides=true) {
    difference(){
        union(){
            translate([0,0,PLATE_HEIGHT*(height*3-1)])
                    brick(width,1,1, true, false);
            brick(width,1,1, false, true);
        //    translate([PLAY/2+tolerance, PLAY/2+tolerance ,PLATE_HEIGHT])
        //        cube([UNIT_LENGTH*width-PLAY-2*tolerance,1.5-PLAY,PLATE_HEIGHT*(height*3-1)]);
            
            for( i = [0 : width-1] ){
                translate([i*UNIT_LENGTH,0,PLATE_HEIGHT]) linear_extrude(PLATE_HEIGHT*(height*3-1)) profile();
            }
            
            if(sides) {
                translate([PLAY/2+tolerance, PLAY/2+tolerance,PLATE_HEIGHT])
                    cube([1.5,UNIT_LENGTH*1-PLAY-2*tolerance,PLATE_HEIGHT*(height*3-1)]);
                translate([UNIT_LENGTH*width-1.5-PLAY/2-tolerance, PLAY/2+tolerance,PLATE_HEIGHT])
                    cube([1.5,UNIT_LENGTH*1-PLAY-2*tolerance,PLATE_HEIGHT*(3*height-1)]);

            }
        }
        cube([width*UNIT_LENGTH,PLAY/2+tolerance,height*3*PLATE_HEIGHT+1]);
        cube([PLAY/2+tolerance,1*UNIT_LENGTH,height*3*PLATE_HEIGHT+1]);
        translate([width*UNIT_LENGTH-(PLAY/2+tolerance),0,0]) 
            cube([PLAY/2+tolerance,1*UNIT_LENGTH,height*3*PLATE_HEIGHT+1]);
    }
}
//profile();

module container_v2(width=6, height=3, sides=true) {
    translate([0,0,PLATE_HEIGHT*(height*3-1)])
            brick(width,1,1, true, false);
    brick(width,1,1, false, true);
    translate([PLAY/2+tolerance, PLAY/2+tolerance ,PLATE_HEIGHT])
        cube([UNIT_LENGTH-PLAY-2*tolerance,1.5-PLAY,PLATE_HEIGHT*(height*3-1)]);
    translate([PLAY/2+tolerance+2*UNIT_LENGTH, PLAY/2+tolerance ,PLATE_HEIGHT])
        cube([UNIT_LENGTH-PLAY-2*tolerance,1.5-PLAY,PLATE_HEIGHT*(height*3-1)]);
    translate([PLAY/2+tolerance+4*UNIT_LENGTH, PLAY/2+tolerance ,PLATE_HEIGHT])
        cube([UNIT_LENGTH-PLAY-2*tolerance,1.5-PLAY,PLATE_HEIGHT*(height*3-1)]);
    
    translate([PLAY/2+tolerance+1*UNIT_LENGTH, UNIT_LENGTH-1.5 ,PLATE_HEIGHT])
        cube([UNIT_LENGTH-PLAY-2*tolerance,1.5-PLAY,PLATE_HEIGHT*(height*3-1)]);
    translate([PLAY/2+tolerance+3*UNIT_LENGTH, UNIT_LENGTH-1.5, PLATE_HEIGHT])
        cube([UNIT_LENGTH-PLAY-2*tolerance,1.5-PLAY,PLATE_HEIGHT*(height*3-1)]);
    translate([PLAY/2+tolerance+5*UNIT_LENGTH, UNIT_LENGTH-1.5, PLATE_HEIGHT])
        cube([UNIT_LENGTH-PLAY-2*tolerance,1.5-PLAY,PLATE_HEIGHT*(height*3-1)]);
        
    translate([PLAY/2+tolerance, PLAY/2+tolerance,PLATE_HEIGHT])
        cube([1.5,UNIT_LENGTH*1-PLAY-2*tolerance,PLATE_HEIGHT*(height*3-1)]);
    translate([UNIT_LENGTH*1-.75, PLAY/2+tolerance,PLATE_HEIGHT])
        cube([1.5,UNIT_LENGTH*1-PLAY-2*tolerance,PLATE_HEIGHT*(3*height-1)]);
    translate([UNIT_LENGTH*2-.75, PLAY/2+tolerance,PLATE_HEIGHT])
        cube([1.5,UNIT_LENGTH*1-PLAY-2*tolerance,PLATE_HEIGHT*(3*height-1)]);
    translate([UNIT_LENGTH*3-.75, PLAY/2+tolerance,PLATE_HEIGHT])
        cube([1.5,UNIT_LENGTH*1-PLAY-2*tolerance,PLATE_HEIGHT*(3*height-1)]);
    translate([UNIT_LENGTH*4-.75, PLAY/2+tolerance,PLATE_HEIGHT])
        cube([1.5,UNIT_LENGTH*1-PLAY-2*tolerance,PLATE_HEIGHT*(3*height-1)]);
    translate([UNIT_LENGTH*5-.75, PLAY/2+tolerance,PLATE_HEIGHT])
        cube([1.5,UNIT_LENGTH*1-PLAY-2*tolerance,PLATE_HEIGHT*(3*height-1)]);
    translate([UNIT_LENGTH*width-1.5-PLAY/2-tolerance, PLAY/2+tolerance,PLATE_HEIGHT])
        cube([1.5,UNIT_LENGTH*1-PLAY-2*tolerance,PLATE_HEIGHT*(3*height-1)]);

}

//container(6,4);
//container(4,3);

//container_v2(6,4);
//container_v3(8,5,true);
// container_v3(6,5,true);
