// library for parametric involute gears 
// Author: Rudolf Huttary, Berlin 
// Jan 2015

$fn = 500; 
iterations = 10; // increase for enhanced resolution beware: large numbers will take lots of time!

// default values
// z = 10; // teeth - beware: large numbers may take lots of time!
// m = 1;  // modulus
// x = 0;  // profile shift
// h = 4;  // face_width	respectivly axial height
// w = 20; // profile angle
// clearance  // assymmetry of tool to clear tooth head and foot
//    = 0.1; // for external splines
//    = -.1  // for internal splines 
// w_bevel = 45; // axial pitch angle
// w_helix = 45; // radial pitch angle 

// use this prototype:
// gear(m = modulus, z = Z, x = profile_shift, w = alpha, h = face_width);

di = 20; 
knob_r = 2.5-.125;  // took away .2
knob_h = 4;
servo_r = 2.5; // was 2.45 added .250/2 added .250/2 again
servo_h = 4;

// 2 Gears for knobs 

module knob_gear()
{
    difference(){       
        gear_helix(z = 16, m = 1.2, h = knob_h, w_helix = -20, clearance = 0.1); 
        cylinder(h=knob_h,r=knob_r, center= true);
    }
}

translate([di, di, knob_h])     knob_gear();
translate([-di, di, knob_h])    knob_gear();


// 2 gears for servos

module servo_gear() {
    difference(){   
        gear_helix(z = 16, m = 1.2, h = servo_h, w_helix = 20, clearance = 0.1); 
        cylinder(h=servo_h,r=servo_r, center= true);
    }
    
    // keyway
    
    translate([1.7,0-4, -servo_h/2]) cube(size=[3,8,servo_h]);
    translate([-4.7,0-4, -servo_h/2]) cube(size=[3,8,servo_h]);
}

translate([di, -di, servo_h])       servo_gear();
translate([-di, -di, servo_h])      servo_gear();

//
// Abandon hope all ye who edit past this comment !!!
//

// === modules for internal splines
module Gear(m = 1, z = 10, x = 0, h = 4, w = 20, D = 13, clearance = -.1, center = true) 
{
   difference()
   {
		cylinder(r = D/2, h = h, center = center);
		gear(m, z, x, h+1, w, center = center, clearance = clearance); 
	}
}

module Gear_herringbone(m = 1, z = 10, x = 0, h = 4, w = 20, w_helix = 45, D = 13, clearance = -.1) 
{
   difference()
   {
		cylinder(r = D/2, h = h-.01, center = true); // CSG!
		gear_herringbone(m, z, x, h, w, w_helix, clearance = clearance); 
	}
}

module Gear_helix(m = 1, z = 10, x = 0, h = 4, w = 20, w_helix = 45, D = 13, clearance = -.1) 
{
   difference()
   {
		cylinder(r = D/2, h = h-.01, center = true); // CSG!
		gear_helix(m, z, x, h, w, w_helix, clearance = clearance); 
	}
}

module Gear_bevel(m = 1, z = 10, x = 0, h = 4, w = 20, w_bevel = 45, w_helix = 0, D = 13, clearance = -0.1, center = true)
{
   rotate([0, 180, 0])
   difference()
   {
		cylinder(r = D/2, h = h-.01, center = true); // CSG!
		gear_bevel(m, z, x, h, w, w_bevel, w_helix, clearance = clearance, center = center); 
	}
}


// === modules for external splines
module gear_herringbone(m = 1, z = 10, x = 0, h = 4, w = 20, w_helix = 45, clearance = 0.1)
{
   for(i=[0, 1])
   mirror([0,0,i])
   translate([0, 0, -0.001]) // CSG!
   gear_helix(m, z, x, h/2, w, w_helix, center = false, clearance = clearance); 
}

module gear_helix(m = 1, z = 10, x = 0, h = 4, w = 20, w_helix = 45, center = true, clearance = 0.1)
{
   gear_info(m, z, x, h, w, w_helix, 0, clearance); 
	r_wk = m*z/2 + x; 
   tw = (h * w_helix)/ r_wk  ; 
	linear_extrude(height = h, center = center, twist = tw, slices = ceil(10*h), convexity = z)
   gear_(m, z, x, w, clearance); 
}

module gear_bevel(m = 1, z = 10, x = 0, h = 4, w = 20, w_bevel = 45, w_helix = 0, clearance = 0.1, center = true)
{
   gear_info(m, z, x, h, w, 0, w_bevel, clearance); 
	r_wk = m*z/2 + x; 
   sc = (r_wk-tan(w_bevel)*h)/r_wk; 
   tw = (h * w_helix)/ r_wk  ; 
	linear_extrude(height = h, center = center, twist = tw, scale = [sc, sc], convexity = z)
   gear_(m, z, x, w, clearance); 
}

module gear(m = 1, z = 10, x = 0, h = 4, w = 20, clearance = 0.1, center = true)
{
   gear_info(m, z, x, h, w, clearance = clearance); 
	linear_extrude(height = h, center = center, convexity = z)
   gear_(m, z, x, w, clearance); 
}

module gear_info(m = 1, z = 10, x = 0, h = 0, w = 20, helix_angle = 0, bevel_angle = 0, clearance = 0.1)
{
  	r_wk = m*z/2 + x; 
   	dy = m;  
  	r_kk = r_wk + dy;   
  	r_fk = r_wk - dy;  
  	r_kkc = r_wk + dy *(1-clearance/2);   
  	r_fkc = r_wk - dy *(1+clearance/2);  
   echo(str ("modulus = ", m)); 
   echo(str ("Z = ", z)); 
   echo(str ("profile angle = ", w, "°")); 
   echo(str ("d = ", 2*r_wk)); 
   echo(str ("d_outer = ", 2*r_kk, "mm")); 
   echo(str ("d_inner = ", 2*r_fk, "mm")); 
   echo(str ("height = ", h, "mm")); 
   echo(str ("clearance factor = ", clearance)); 
   echo(str ("d_outer_cleared = ", 2*r_kkc, "mm")); 
   echo(str ("d_inner_cleared = ", 2*r_fkc, "mm")); 
   echo(str ("helix angle = ", helix_angle, "°")); 
   echo(str ("bevel angle = ", bevel_angle, "°")); 
   echo("================"); 
}

// === from here 2D stuff == 
module gear_(m = 1, z = 10, x = 0, w = 20, clearance = 0.1)
{
  	r_wk = m*z/2 + x; 
   	dy = m;  
  	r_fkc = r_wk - dy *(1+clearance/2)+.01;  //.01 has numerical reason!
   union()
	{
		for(i = [0:z-1]) 
			rotate([0, 0, i*360/z])
			Tooth(m, z, x, w, clearance);
     circle(r_fkc); 
	}
}

module Tooth(m = 1, z = 10, x = 0, w = 20, clearance = 0)
{
	union()
	{
		Tooth_half(m, z, x, w, clearance); 
		mirror([1, 0, 0])
		Tooth_half(m, z, x, w, clearance); 
	}
}

module Tooth_half(m = 1, z = 10, x = 0, w = 20, clearance = 0)
{
  	r_wk = m*z/2 + x; 
  	r_fkc = r_wk - m *(1+clearance/2);   
	du = z*m/360*PI; 
	dw = 360/z; 
 	difference()
	{
		cake_piece(m, z, x, w, clearance); 
      	union()
		{
			for(i = [-dw:dw/iterations:dw])
			  rotate([0, 0, i])
			  translate([i*du, 0, 0])
			Tool(m, z, x, w, clearance); 
		}
	}
}

module Tool(m = 1, z = 10, x = 0, w = 20, clearance = 0)
{
   p = m*PI; 
   dy = 2*m;  
   dx = dy * tan(w);  
   ddx = dx/2 * clearance/2; 
   ddy = dy/2 * clearance/2; 
	r_wk = m*z/2 + x; 
   polygon(points = [
		[0, r_wk+dy], 
		[0, r_wk+dy/2-ddy], 
		[p/4-dx/2 + ddx, r_wk+dy/2 - ddy], 
		[p/4+dx/2 + ddx, r_wk-dy/2 - ddy], 
		[p/2,            r_wk -dy/2 - ddy],
		[p/2,            r_wk +dy/2 - ddy],
		[p/2,            r_wk+dy], 
		]);  
}

module cake_piece(m = 1, z = 10, x = 0, w = 20, clearance = 0)
{
	$fn = 100; 
	r_wk = m*z/2 + x; 
   p = m*PI; 
   dy = p/2*cos(w);  
   r_kk = r_wk + dy*(1+clearance);   
   intersection()
   {
     half_circle(r_kk); 
     rotate([0, 0, 180])
     rotate([0, 0, -181/z]) // 181 has numerical reason!
     half_circle(r_kk); 
	}
}

module half_circle(r)
{
	intersection()
  	{
		circle(r = r); 
     translate([0, -r, 0])
     square(2*r); 
	}
}

