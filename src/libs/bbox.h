#ifndef BBOX_H
#define BBOX_H
#include <gb/gb.h>
#include <stdio.h>

extern struct BoundingBox {
    // Tile set data
    int16_t x_left;
    int16_t y_top;
    int16_t width;
    int16_t height;
};

extern struct BoundingBox init_bbox(int16_t x_left, int16_t y_top, int16_t width, int16_t height);
extern void update_bbox(struct BoundingBox *box, int16_t new_x, int16_t new_y);
extern BOOLEAN check_collision(struct BoundingBox bbox_1, struct BoundingBox bbox_2);

#endif /* BBOX_ */