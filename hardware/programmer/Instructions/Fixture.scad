// Rev B - ONE PCB. This is an insulating printed/machined locator, not another PCB.
// Local module top-left [7.5,0]; PCB coordinates = local XY + [100,50].
// Z0 = PCB top. PCB thickness 1.6. Probe terminal tip Z-1.8 (0.2 below PCB).
// INGUN GKS-080-0001 p1: total22.3, lower terminal3.0, collar at12.0 from bottom tip.
// Therefore barrel bottom Z1.2; collar Z10.2; free tip Z20.5; working module Z17.5.
part="locator"; // locator, upper_bar, lower_bar, assembly
$fn=64;
collar_z=10.20; module_bottom=17.50; module_height=2.50;
bore=.70; fit=.05; // Precision finish to actual barrels/module; FDM accuracy is not assumed.
contacts=[[4,31.5],[5,31.5],[11,31.5],[12,31.5],[14.5,30.1],[14.5,18.1],[.5,26.1],[14.5,11.1],[12.27,21.53],[4.21,11.42]];
holes=[[2.5,12.5],[27.5,12.5],[2.5,33.5],[27.5,33.5]];
module locator(){difference(){union(){
 // Lower block supports the barrels/collars; no PCB carries axial load at the thin tails.
 difference(){translate([0,10,0])cube([30,26,collar_z]);
  for(c=contacts)translate([7.5+c[0],c[1],-.1])cylinder(h=collar_z+.2,d=bore);
 }
 // Upper frame locates the module and supports its underside at the working height.
 difference(){translate([0,10,collar_z])cube([30,26,module_bottom-collar_z]);
  translate([7.5-fit,9.9,collar_z-.1])cube([15+2*fit,22.1+fit,module_bottom-collar_z+.2]);
 }
 for(y=[16,24])for(x=[6.5,20.9])translate([x,y,collar_z])cube([2.6,1,module_bottom-collar_z]);
 for(hp=holes)translate([hp[0],hp[1],0])cylinder(h=module_bottom+module_height,d=5);
 }for(hp=holes)translate([hp[0],hp[1],-.1])cylinder(h=module_bottom+module_height+.2,d=3.3);
}}
module bar(y,h){difference(){translate([0,y,0])cube([30,h,3]);
 for(hp=holes)if(hp[1]>=y && hp[1]<=y+h)translate([hp[0],hp[1],-.1])cylinder(h=3.2,d=3.3);
}}
if(part=="locator")locator();
if(part=="upper_bar")bar(10,5);
if(part=="lower_bar")bar(29,7);
if(part=="assembly"){
 color("gray")locator();
 color("green",.5)translate([7.5,0,module_bottom])cube([15,32,module_height]);
 translate([0,0,module_bottom+module_height]){bar(10,5);bar(29,7);}
 for(c=contacts)color("gold")translate([7.5+c[0],c[1],1.2])cylinder(h=19.3,d=.35);
}
