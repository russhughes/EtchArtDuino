
length = 80;            // box lenght
width = 35;             // box width
height = 20;            // box, height

joy = 28;               // joystick hole diameter
joy_offset = 15;         // x offset for joysitck

so_d = 6;               // stand off diameter

disp_offset = 15;        // display offset from end
disp_w = 27;            // display width
disp_l =16;             // display length

disp_hdist = 20;        // display horizontal mounting hole distance
disp_vdist =  23;       // display vertial mount hole distance

module standoff(sh=6, sd=6)
{
    translate([0,0,-1])
    {
        difference() 
        {
            // standoff
            
            cylinder(h=sh,d=sd, center= false);
            
            // screw hole
            
            cylinder(h=sh,d=sd/2, center= false);
        }
    }
}

module base() 
{
    // four standoffs for joystick (25x20mm) 
    
    translate([0,0,0])      standoff(sh=7, sd=so_d);
    translate([25,0,0])     standoff(sh=7, sd=so_d);
    translate([0,20,0])     standoff(sh=7, sd=so_d);
    translate([25,20,0])    standoff(sh=7, sd=so_d);
}    

module wire()
{
    cube([2,10,5]);
}

module box()
{
    difference()
    {
        // outer shell
        
        cube([length, width, height]);
        
        // inner wall
        
        translate([1,1,1])
            cube([length-2, width-2, height]);
        
        // wire hole
        
        translate([0,width/2-5, 10])
            wire();

        // Joystick hole
        
        translate([length/2+joy_offset-0.5,width/2-0.5,0])
            cylinder(h=2,d=joy, center=false);
        
        // display hole
        
        translate([disp_offset ,width/2-disp_w/2,0])    cube([disp_l,disp_w,1]);
        
        // display mounting holes
        
        translate([disp_offset ,width/2-disp_w/2,0])
        {
            translate([-2,3,0])     cylinder(h=1,d=2, center= false);
            translate([-2,23,0])    cylinder(h=1,d=2, center= false);
            translate([-2+23,3,0])  cylinder(h=1,d=2, center= false);
            translate([-2+23,23,0]) cylinder(h=1,d=2, center= false);
        }
    }
    
    // corner standoffs
    
    translate([5-1.25,5-1.25,1]) standoff(sh=height-2.1, sd=so_d);
    translate([5-1.25,30+1.5,1]) standoff(sh=height-2.1, sd=so_d);
    
    translate([length-3.75,5-1.25,1]) standoff(sh=height-2.1, sd=so_d);
    translate([length-3.75,30+1.5,1]) standoff(sh=height-2.1, sd=so_d);
    
    // proto board standoff
    

}

module bottom()
{
    difference()
    {
        // bottom
        
        union() 
        {
            cube([length, width, 1]);
        
            // joystick mounting base

            translate([length/2-25/2+joy_offset, width/2-20/2,1]) 
                base();
            
            // protoboard standoff
            
            translate([10,width/2,1]) standoff(7, sd=so_d);

    
            // lengthwise bottom ridges
    
            translate([1.1,1.1,1])       cube([length-2.2, 2, 2]);
            translate([1.1,width-3.1,1]) cube([length-2.2, 2, 2]);
    
            // horizontal botton ridges
    
            translate([1.1,1.1,1])         cube([2, width-2.2, 2]);
            translate([length-3.1,1.1,1])  cube([2, width-2.2, 2]);
        }  
        
        // standoff screw holes
        
        translate([5-1.25,5-1.25,0])        cylinder(h=5,d=so_d/2, center= false);
        translate([5-1.25,30+1.5,0])        cylinder(h=5,d=so_d/2, center= false);
        translate([length-3.75,5-1.25,0])   cylinder(h=5,d=so_d/2, center= false);
        translate([length-3.75,30+1.5,0])   cylinder(h=5,d=so_d/2, center= false);
    }

   
}

box();
translate([0, width*2, 0])   bottom();
