#include "block.h"
#include "playing_field.h"

bool is_block_out_of_bounds(struct block block)
{
	return block.x < PLAYING_FIELD_LEFT_BORDER ||
	       block.x > PLAYING_FIELD_RIGHT_BORDER ||
	       block.y < PLAYING_FIELD_TOP || block.y > PLAYING_FIELD_BOTTOM;
}
