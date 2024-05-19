#include "bbox.h"
#include <gb/gb.h>
#include <stdio.h>

extern struct BoundingBox init_bbox(int16_t x_left, int16_t y_top, int16_t width, int16_t height) {
    struct BoundingBox output;
    output.x_left = x_left;
    output.y_top = y_top;
    output.width = width;
    output.height = height;
    return output;
}
extern void update_bbox(struct BoundingBox *box, int16_t new_x, int16_t new_y) {
    box->x_left = new_x;
    box->y_top = new_y;
}

extern BOOLEAN check_collision(struct BoundingBox bbox_1, struct BoundingBox bbox_2) {
    BOOLEAN x_bbox_1_overlaps = bbox_1.x_left + bbox_1.width < bbox_2.x_left;
    BOOLEAN x_bbox_2_overlaps = bbox_2.x_left + bbox_2.width < bbox_1.x_left;

    // Check if there is no overlap on the x axis
    if (x_bbox_1_overlaps || x_bbox_2_overlaps) {
        return FALSE;
    }
    BOOLEAN y_bbox_1_overlaps = bbox_1.y_top + bbox_1.height < bbox_2.y_top;
    BOOLEAN y_bbox_2_overlaps = bbox_2.y_top + bbox_2.height < bbox_1.y_top;
    // Check if there is no overlap on the y axis
    if (y_bbox_1_overlaps || y_bbox_2_overlaps) {
        return FALSE;
    }
    // If there is overlap both on x and y axes, then there is a collision
    return TRUE;
}