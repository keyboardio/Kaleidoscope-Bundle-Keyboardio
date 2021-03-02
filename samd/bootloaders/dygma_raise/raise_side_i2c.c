/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.
  Copyright (c) 2015 Atmel Corporation/Thibaut VIARD.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "board_driver_i2c.h"
#include "raise_side_i2c.h"

#ifdef CONFIGURE_RAISE_SIDE

extern uint8_t rxBuffer[6];

bool readSideBootloaderKey()
{
    // reads with no reg setup are a key read. This also doesn't calculate or check the checksum
    for( int i = 0; i < 10; i ++ )
    {
    i2c_requestFrom(LEFT_SIDE_ADDRESS, 6, true);
    if(rxBuffer[0] == 1 && rxBuffer[1] == 1) // key pressed and key is escape
        return true;

    i2c_requestFrom(RIGHT_SIDE_ADDRESS, 6, true);
    if(rxBuffer[0] == 1 && rxBuffer[1] == 1) // key pressed and key is delete
        return true;
    }
    SIDE_POWER_off();
    return false;
}

void configureRaiseSide()
{
  // turn on power to the sides
  SIDE_POWER_init();
  SIDE_POWER_on();

  // initialise I2C
  i2c_init(200000);
}

#endif
