/*
 * motors.inc
 *
 *  Created: 12/25/2022 9:12:59 PM
 *   Author: USER
 */ 
 .macro STSw
cli
STS @0, @1
STS @0-1, @2
sei
.endmacro

.macro LDSw
cli
LDS @1, @2-1
LDS @0, @2
sei
.endmacro