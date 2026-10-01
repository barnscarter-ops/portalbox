// CYD 3.5 enclosure, revision 0.2: physical fit unverified.
// Rear-view coordinates: USB left, I2C upper right. Units: mm.
// LCDWiki E32R35T_Size.pdf is a family reference, not proof of owner-board fit.
part="assembly"; // bezel, shell, rail_cover, stylus_cap, pod, pod_lid, divider, button, fit_coupon
with_pod=false;
$fn=48;
pcb=[101.5,55.5]; gap=0.5; wall=2.4;
W=pcb[0]+2*(gap+wall); H=pcb[1]+2*(gap+wall);
O=gap+wall; depth=31; seam=3;
// Centers from the dimensioned rear drawing after rotating into landscape.
// Owner board checked 2026-09-30: speaker faces rear; BAT faces inward.
// Both stay enclosed. These lists include only outward-facing edge connectors.
upper_ports=[[29.44,8],[64.1,12],[84.1,12]]; // GPIO39/35, SPI, I2C
lower_ports=[[40.02,13],[57.89,16]]; // UART, microSD
port_z=11; port_h=6.5; // provisional vertical offsets / clearances
mounts=[[O+3.5,O+3.8],[O+98,O+3.8],[O+3.5,O+51.7],[O+98,O+51.7]];
buttons=[[O+3.26,O+41.47],[O+3.26,O+14.03]];
fan=[29,44]; fan_spacing=20; // Sunon has THREE mounting holes; fourth position unused
fan_holes=[[-10,-10],[-10,10],[10,10]];
rail_x=15; rail_y=18; rail_len=W-rail_x; rail_base=2; rail_h=4;
rail_slop=0.35; // profile allowance; mating gap varies with seating height
stylus_len=87.31; stylus_bore=8.8; tube_len=98;
tube_y=-5; tube_z=depth-3; tube_x=7;
pod_x=44; pod_y=5; pod_z=depth+rail_slop;
pod_size=[66,56,14]; battery=[60,50,7.3];
boost=[52,34,16.6]; boost_size=[45,23,10];
module rounded(w,h,z,r=3){linear_extrude(z) hull() for(x=[r,w-r],y=[r,h-r]) translate([x,y]) circle(r);}
module bore(x,y,z,d,h){translate([x,y,z]) cylinder(d=d,h=h);}
module screw_cuts(z,h,d=3.3){for(p=mounts) bore(p[0],p[1],z,d,h);}
module bezel(){difference(){
 union(){rounded(W,H,seam); for(p=mounts) bore(p[0],p[1],seam-0.1,6.4,4.5);}
 // Oversize touch-visible window, with no pressure on resistive touch film.
 translate([W/2-78.6/2,H/2-51.2/2,-1]) rounded(78.6,51.2,5,0.8);
 screw_cuts(-1,10); for(p=mounts) bore(p[0],p[1],-0.01,6.2,1.4);
}}
// Recessed dovetail: reinforcement projects inward; rear surface stays at z=31.
module female_track(){difference(){
 translate([rail_x,rail_y,depth-rail_base-rail_h-0.1]) cube([rail_len,13,rail_base+rail_h+0.1]);
 translate([rail_x+2,rail_y+6.5,depth-rail_h]) rotate([0,90,0])
 linear_extrude(rail_len+1) polygon([[0,-4.6],[-rail_h,-3.1],[-rail_h,3.1],[0,4.6]]);
 // M3 nut recess under stop; lock screw in shoe threads into captive nut.
 for(x=[rail_x+8,40]){bore(x,rail_y+6.5,depth-8,4.2,4);
 bore(x,rail_y+6.5,depth-4.1,3.2,5);}
}}
module track_cuts(){
 translate([rail_x+2,rail_y+6.5,depth-rail_h]) rotate([0,90,0])
 linear_extrude(rail_len+1) polygon([[0,-4.6],[-rail_h,-3.1],[-rail_h,3.1],[0,4.6]]);
 translate([rail_x+1.8,rail_y+0.1,depth-1.45]) cube([rail_len,12.8,2]);
 for(x=[rail_x+8,40]){bore(x,rail_y+6.5,depth-8,4.2,4);bore(x,rail_y+6.5,depth-4.1,3.2,5);}
}
module boost_cradle(){
 // Insulated pad and retaining corner blocks; actual PCB fit pending.
 for(x=[boost[0]-1.5,boost[0]+boost_size[0]],y=[boost[1]-1.5,boost[1]+boost_size[1]])
 translate([x,y,boost[2]+8.5]) cube([1.5,1.5,depth-boost[2]-8.5]);
 for(y=[boost[1]-1.5,boost[1]+boost_size[1]])
 translate([boost[0],y,boost[2]+10]) cube([boost_size[0],1.5,depth-boost[2]-10]);
}
module shoe(len=80){rotate([0,90,0]) linear_extrude(len)
 polygon([[0,-4.6+rail_slop],[-rail_h+rail_slop,-3.1+rail_slop],[-rail_h+rail_slop,3.1-rail_slop],[0,4.6-rail_slop]]);}
module edge_cuts(){
 for(p=upper_ports) translate([O+p[0]-p[1]/2,H-wall-1,port_z-port_h/2]) cube([p[1],wall+3,port_h]);
 for(p=lower_ports) translate([O+p[0]-p[1]/2,-1,port_z-port_h/2]) cube([p[1],wall+3,port_h]);
 translate([-1,O+27.75-6,port_z-3.5]) cube([wall+3,12,7]);
 // intake slots on the right edge, behind the PCB components
 for(y=[14:7:28]) translate([W-wall-1,y,20]) cube([wall+3,3.5,6]);
 // Charge and keyed battery socket reservations; final connector SKUs pending.
 translate([72,H-wall-1,20]) cube([14,wall+3,8]);
 translate([W-wall-1,37,20]) cube([wall+3,10,7]);
}
module tube(){difference(){
 union(){translate([tube_x,tube_y,tube_z]) rotate([0,90,0]) cylinder(d=12.8,h=tube_len);
 translate([tube_x,-2,depth-8]) cube([tube_len,4,7]);}
 // Tube has a 2.4 mm blind end wall; cap retains the stylus at handle end.
 translate([tube_x+2.4,tube_y,tube_z]) rotate([0,90,0]) cylinder(d=stylus_bore,h=tube_len+1);
 // Two opposed bayonet entry slots and quarter-turn retaining grooves.
 for(a=[0,180]) translate([tube_x+tube_len-4.2,tube_y,tube_z]) rotate([a,0,0])
 translate([0,-1,-5.4]) cube([5.5,2,1.7]);
 for(a=[0,180]) translate([tube_x+tube_len-3.2,tube_y,tube_z]) rotate([0,90,0])
 rotate([0,0,a]) rotate_extrude(angle=95) translate([4.05,0]) square([1.3,1.6]);
 // cross-hole for a bought silicone tether / cord
 translate([tube_x+tube_len-4,-2,tube_z+5]) rotate([90,0,0]) cylinder(d=2,h=8);
}}
module shell(){difference(){union(){
 difference(){translate([0,0,seam]) rounded(W,H,depth-seam);
 translate([wall,wall,seam-0.1]) rounded(W-2*wall,H-2*wall,depth-seam-wall+0.1,1.2);}
 // Four columns join shell to bezel and support board at its hole locations.
 for(p=mounts) bore(p[0],p[1],9.1,6.4,depth-9.1);
 // Rear button guide sleeves end safely short of switches.
 for(p=buttons) bore(p[0],p[1],16,8,depth-16);
 // Fan frame seats directly against rear skin; no posts invade its envelope.
 tube(); female_track(); boost_cradle();
}
 edge_cuts(); track_cuts();
 screw_cuts(8.9,10,2.6); // blind M3 pilots; screws installed from bezel
 // Press-fit insert seats at front ends of columns, not behind battery.
 screw_cuts(9.09,4.2,4.2);
 for(p=buttons) bore(p[0],p[1],15,4.8,depth-13);
 // Fan exhaust with hub and eight integrated spokes, no exposed rotor.
 translate([fan[0],fan[1],depth-wall-0.1]) difference(){
 cylinder(d=23,h=wall+1); cylinder(d=7,h=wall+2);
 for(a=[0:45:315]) rotate([0,0,a]) translate([-1.1,0,-1]) cube([2.2,15,wall+4]);}
 for(v=fan_holes) bore(fan[0]+v[0],fan[1]+v[1],depth-13,2.8,15);
}}
module rail_cover(){difference(){union(){
 shoe(rail_len-2); translate([0,-6.3,rail_h-rail_slop-1.4]) rounded(rail_len-2,12.6,1.4,1);
} bore(6,0,-1,3.3,9);}}
module stylus_cap(){difference(){union(){
 cylinder(d=12.8,h=3); translate([0,0,3]) cylinder(d=8.2,h=4);
 for(a=[0,180]) rotate([0,0,a]) translate([3.8,-0.7,5]) cube([1.3,1.4,1.2]);
 translate([5,-2,0]) cube([4,4,3]);}
 bore(7,0,-1,2,5);
} // bayonet cap; test actual fit before fitting stylus
}
module button(){union(){cylinder(d=4.2,h=18.5); cylinder(d=7,h=2);}}
// Battery-only pod; charger stays in main case when pod is detached.
module pod(){difference(){union(){
 difference(){rounded(pod_size[0],pod_size[1],pod_size[2]);
 translate([wall,wall,wall]) rounded(pod_size[0]-2*wall,pod_size[1]-2*wall,pod_size[2],1);}
 for(x=[4,pod_size[0]-4],y=[4,pod_size[1]-4]) bore(x,y,11,6,3);
 // Mechanical dovetail foot, on outside of pod floor
 translate([0,rail_y+6.5-pod_y,-rail_h+rail_slop]) shoe(58);
 // External lock tab: screw path never enters the pouch footprint.
 translate([-8,rail_y+6.5-pod_y-6,-rail_h+rail_slop]) cube([8.2,12,7]);
}
 // Lid pilots, kept above pouch compartment.
 for(x=[4,pod_size[0]-4],y=[4,pod_size[1]-4]) bore(x,y,11,2.4,5);
 // M3 lock screw reaches the right-hand insert in the fixed track.
 bore(-4,rail_y+6.5-pod_y,-5,3.3,10);
 // Battery lead exits through a strain-relieved grommet.
 translate([pod_size[0]-wall-1,35,8]) rotate([0,90,0]) cylinder(d=5,h=wall+3);
} }
module pod_lid(){difference(){rounded(pod_size[0],pod_size[1],2.4);
 for(x=[4,pod_size[0]-4],y=[4,pod_size[1]-4]) bore(x,y,-1,2.8,5);
}}
module divider(){difference(){rounded(pod_size[0]-2*wall-0.6,pod_size[1]-2*wall-0.6,1.6,1);
 // cable passage from pouch JST lead; smooth before assembly
 translate([3,1,-1]) rounded(10,6,4,1);
}}
module fit_coupon(){difference(){union(){cube([28,17,3]); translate([3,8.5,3]) shoe(22);}
 bore(10,8.5,-1,3.3,12);}
 translate([0,24,0]) difference(){cube([28,16,7]);translate([0,8,3]) rotate([0,90,0])
 linear_extrude(30) polygon([[0,-4.6],[-4,-3.1],[-4,3.1],[0,4.6]]);}
 translate([38,0,0]) difference(){cube([14,14,15]);bore(7,7,-1,stylus_bore,18);}
}
module assembly(){
 color([0.15,0.18,0.21]) bezel(); color([0.22,0.25,0.28]) shell();
 // illustrative screen and PCB envelopes, not fabrication geometry
 color([0.02,0.06,0.09]) translate([W/2-77.84/2,H/2-50.56/2,3.2]) cube([77.84,50.56,1.2]);
 if(with_pod){color([0.32,0.35,0.38]) translate([pod_x,pod_y,pod_z]) pod();
 color([0.19,0.22,0.25]) translate([pod_x,pod_y,pod_z+pod_size[2]]) pod_lid();}
 else color([0.32,0.35,0.38]) translate([rail_x+2,rail_y+6.5,depth-rail_h+rail_slop]) rail_cover();
 color([0.32,0.35,0.38]) translate([tube_x+tube_len+3,tube_y,tube_z]) rotate([0,-90,0]) stylus_cap();
}
if(part=="assembly") assembly();
else if(part=="bezel") bezel();
else if(part=="shell") shell();
else if(part=="rail_cover") rail_cover();
else if(part=="stylus_cap") stylus_cap();
else if(part=="pod") pod();
else if(part=="pod_lid") pod_lid();
else if(part=="divider") divider();
else if(part=="button") button();
else if(part=="fit_coupon") fit_coupon();
else assert(false,"Unknown part");
