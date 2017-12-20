
height=10;              // height of pocket
width=95.3;             // width of pocket
length=82.3;            // length of pocket
thick=3;                // thickness of wall
supports=5;             // support shelf thickness 

owidth=width+thick*2;   // pocket outside width
olength=length+thick*2; // pocket outside length
oheight=height+thick;   // pocket outside height

so_d = 6;               // stand off diameter
shelf_w = owidth+15+15;          // board shelf width
shelf_l = 45;           // board shelf length

// board stand off

module stand_off()
{
    translate([0,0,-1])
    {
        difference() 
        {
            cylinder(h=6,d=so_d, center= false);
            cylinder(h=6,d=so_d/2.5, center= false);
        }
    }
}

// Servo board holder

module servo_board()
{
    //cube([36, 38, 5]);
    translate([3,9,5]) stand_off();
    translate([3,35,5]) stand_off();
    translate([33,9,5]) stand_off();
    translate([33,35,5]) stand_off();
}

// Cpu board holder

module cpu_board()
{
    //cube([56, 40, 6]);
    translate([4,6,5]) stand_off();
    translate([56-4,6,5]) stand_off();
    
    translate([4,37,5]) stand_off();
    translate([56-4,37,5]) stand_off();
    
}

// Board shelf

module shelf()
{
    translate([0,0,0]) 
    {
        cube([shelf_w, shelf_l, 5]);
        translate([2,0,0]) servo_board();
        translate([130-33-3, 0, 0]) servo_board();
        translate([38, 0, 0]) cpu_board();
    }
}


difference () {
union() {
difference()
{
    cube(size = [owidth,olength,oheight], center = false);
    translate([thick,thick,thick+1]) 
        cube(size = [width,length,height+4], center = false);
}

// left servo base
translate([-15,0,0])
    cube([15,42.5, oheight], center = false);

// left servo mount bottom tower
translate([-15,.05,0])
    cube([15,7, oheight+14], center = false);

// left servo mount top tower
translate([-15,28+7.5,0])
    cube([15,7, oheight+14], center = false);

// left servo controller board
//translate([-5,56.3,0])
//    cube([5,32, supports], center = false);

// right servo base
translate([owidth,0,0])
    cube([15,42.5, oheight], center = false);
    
// right servo mount bottom tower
translate([owidth,.05,0])
    cube([15,7, oheight+14], center = false);
    
// right servo mount top tower
translate([owidth,28+7.5,0]) 
    cube([15,7, oheight+14], center = false);
}

// left servo base servo cutout
translate([-15,7,oheight-5])
    cube([15+thick,28.5, 5], center = false);

// right servo base servo cutout
translate([owidth-thick,7, oheight-5])
    cube([15+thick,28.5, 5], center = false);
}

translate([-15,-shelf_l, 0]) 
    shelf();
