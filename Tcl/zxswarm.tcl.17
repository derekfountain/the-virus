#!/usr/bin/wish

set ::NUM_BOIDS 30
set ::SPEED 20

set c [canvas .c -width 256 -height 192]
pack $c
focus $c

#expr {srand(0)}
array set dots {}
for { set i 0 } { $i < $::NUM_BOIDS } { incr i } {
    set dots($i,graphic) [$c create rectangle 0 0 1 1]
    set dots($i,velocity) [list 0.01 0.01]

    set dots($i,position) [list [expr {int(256*rand())}] [expr {int(192*rand())}]]
    set dots($i,position_int) "[expr {round([lindex $dots($i,position) 0])}] [expr {round([lindex $dots($i,position) 1])}]"
}
set centre [$c create rectangle 0 0 1 1 -outline yellow]
set goal   [$c create rectangle 0 0 5 5 -outline red]

set goal_x 128
set goal_y 96

bind $c <KeyPress-q> {incr ::goal_y -6; puts "Goal $::goal_x , $::goal_y"}
bind $c <KeyPress-a> {incr ::goal_y  6; puts "Goal $::goal_x , $::goal_y"}
bind $c <KeyPress-o> {incr ::goal_x -6; puts "Goal $::goal_x , $::goal_y"}
bind $c <KeyPress-p> {incr ::goal_x  6; puts "Goal $::goal_x , $::goal_y"}

proc draw_dots {} {
    global dots

    for { set i 0 } { $i < $::NUM_BOIDS } { incr i } {
	$::c moveto $dots($i,graphic) [expr {[lindex $dots($i,position) 0]}] \
	                              [expr {[lindex $dots($i,position) 1]}]
    }
}

# Called with floats:
# 4 fp subtractions
# 2 fp multiplications
# 1 fp addition
# Called with ints:
# same
proc vector_distance { v1 v2 } {
    set x_sq [expr { ([lindex $v1 0]-[lindex $v2 0]) * ([lindex $v1 0]-[lindex $v2 0]) } ]
    set y_sq [expr { ([lindex $v1 1]-[lindex $v2 1]) * ([lindex $v1 1]-[lindex $v2 1]) } ]
    return [expr {$x_sq + $y_sq}]
}

# Only ever called with floats
proc vector_add { v1 v2 } {
    set r [list [expr {[lindex $v1 0]+[lindex $v2 0]}] [expr {[lindex $v1 1]+[lindex $v2 1]}] ]
    return $r
}

# Called with floats or ints
proc vector_sub { v1 v2 } {
    set r [list [expr {[lindex $v1 0]-[lindex $v2 0]}] [expr {[lindex $v1 1]-[lindex $v2 1]}] ]
    return $r
}

# Only ever called with floats
proc vector_mul { v1 m } {
    set r [list [expr {[lindex $v1 0]*$m}] [expr {[lindex $v1 1]*$m}] ]
    return $r
}

# Called with floats or ints
proc vector_div { v1 d } {
    set r [list [expr {([lindex $v1 0]/$d)}] [expr {([lindex $v1 1]/$d)}] ]
    return $r
}


# Rule 2: Boids try to keep a small distance away from other boids
# 30 boids==900 integer operations
#
proc avoid_others { index } {
    global dots

    set MIN_DISTANCE 4
    set boid_velocity [list 0 0]

    # This can be done with integer positioning
    #
    for { set i 0 } { $i < $::NUM_BOIDS } { incr i } {
        if { $i != $index } {
            set distance [vector_distance $dots($i,position_int) $dots($index,position_int)]
	    set distance [expr {round($distance)}]
            if { $distance < $MIN_DISTANCE } {
                set boid_velocity [vector_sub $boid_velocity [vector_sub $dots($i,position_int) $dots($index,position_int)]]
            }
        }
    }

    return $boid_velocity
}



# Restrict how fast the boids can move
# 30 boids==210 fp operattions.
#
proc limit_velocity { index } {
    global dots
    
    set SPEED_LIMIT 16

    set magnitude [vector_distance {0 0} $dots($index,velocity)]
    if { $magnitude > $SPEED_LIMIT } {
	set t [vector_div $dots($index,velocity) $magnitude]
	set dots($index,velocity) [vector_mul $t $SPEED_LIMIT]
    }
}

proc move_all_dots {} {
    global dots

    for { set i 0 } { $i < $::NUM_BOIDS } { incr i } {

	set v2 [avoid_others      $i]
    
	# Extra rule to make the boids follow the red dot.
	# Move 1% closer to it each iteration
	#
	set v4 [vector_sub [list $::goal_x $::goal_y] $dots($i,position)]
	set v4 [vector_div $v4 100]

	set dots($i,velocity) [vector_add $dots($i,velocity) $v2]
	set dots($i,velocity) [vector_add $dots($i,velocity) $v4]

	limit_velocity $i

	set dots($i,position) [vector_add $dots($i,position) $dots($i,velocity)]
	set dots($i,position_int) "[expr {round([lindex $dots($i,position) 0])}] [expr {round([lindex $dots($i,position) 1])}]"
    }

    draw_dots
    $::c moveto $::goal $::goal_x $::goal_y

    # Test: Report position and velocity. half16 package on Spectrum is limited to 65535(ish)
    # so flag up if we go outside that range.
    #
    for { set i 0 } { $i < $::NUM_BOIDS } { incr i } {
	if { [lindex $dots($i,position) 0] < -65000 || [lindex $dots($i,position) 0] > 65000
	     ||
	     [lindex $dots($i,position) 1] < -65000 || [lindex $dots($i,position) 1] > 65000
	     ||
	     [lindex $dots($i,velocity) 0] < -65000 || [lindex $dots($i,velocity) 0] > 65000
	     ||
	     [lindex $dots($i,velocity) 1] < -65000 || [lindex $dots($i,velocity) 1] > 65000
	 } {
	    puts "$i : Pos $dots($i,position), Vel $dots($i,velocity)"
	    puts "$i : Pos $dots($i,position_int), Vel $dots($i,velocity)"
	}
    }

    after $::SPEED move_all_dots
}


after $::SPEED move_all_dots
