#ifndef CONSTANTS_H
#define CONSTANTS_H

#define CONTROL_PORT    (8080)

/*** AKTORIK ******/

#define GREEN_LIGHT_ON             "00"
#define GREEN_LIGHT_OFF            "01"
#define GREEN_LIGHT_BLINK_SLOW     "02"
#define GREEN_LIGHT_BLINK_FAST     "03"
#define GREEN_LIGHT_BLINK_OFF      "04"

#define YELLOW_LIGHT_ON            "05"
#define YELLOW_LIGHT_OFF           "06"
#define YELLOW_LIGHT_BLINK_SLOW    "07"
#define YELLOW_LIGHT_BLINK_FAST    "08"
#define YELLOW_LIGHT_BLINK_OFF     "09"

#define RED_LIGHT_ON               "10"
#define RED_LIGHT_OFF              "11"
#define RED_LIGHT_BLINK_SLOW       "12"
#define RED_LIGHT_BLINK_FAST       "13"
#define RED_LIGHT_BLINK_OFF        "14"

/* Motor signals */
#define MOTOR_ON               	"15"
#define MOTOR_OFF				"16"
#define MOTOR_SLOW				"17"
#define MOTOR_FAST				"18"
#define MOTOR_LEFT				"19"
#define MOTOR_RIGHT         	"20"
#define MOTOR_STOP				"21"

/* Gate signals */
#define GATE_OPEN				"22"
#define GATE_CLOSE				"23"

#endif // CONSTANTS_H
