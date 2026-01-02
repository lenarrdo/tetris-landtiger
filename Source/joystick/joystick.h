#ifndef JOYSTICK_H_
#define JOYSTICK_H_

enum joystick_direction { CENTER, LEFT, DOWN, UP, RIGHT };

void joystick_init(void);
enum joystick_direction joystick_direction(void);

#endif // JOYSTICK_H_
