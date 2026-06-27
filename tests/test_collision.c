#define CLOVE_SUITE_NAME CollisionSuite
#include "clove-unit.h"

#include "collision.h"

CLOVE_TEST(OverlappingRectanglesCollide)
{
    Rectangle first = {10.0f, 10.0f, 20.0f, 20.0f};
    Rectangle second = {20.0f, 20.0f, 20.0f, 20.0f};

    CLOVE_IS_TRUE(RectsOverlap(first, second));
}

CLOVE_TEST(SeparatedRectanglesDoNotCollide)
{
    Rectangle first = {0.0f, 0.0f, 10.0f, 10.0f};
    Rectangle second = {20.0f, 20.0f, 10.0f, 10.0f};

    CLOVE_IS_FALSE(RectsOverlap(first, second));
}

CLOVE_TEST(TouchingEdgesDoNotCollide)
{
    Rectangle first = {0.0f, 0.0f, 10.0f, 10.0f};
    Rectangle second = {10.0f, 0.0f, 10.0f, 10.0f};

    CLOVE_IS_FALSE(RectsOverlap(first, second));
}

CLOVE_TEST(CenteredRectangleUsesExpectedCoordinates)
{
    Rectangle actual = RectCentered((Vector2){50.0f, 40.0f}, 20.0f, 10.0f);

    CLOVE_FLOAT_EQ(40.0f, actual.x);
    CLOVE_FLOAT_EQ(35.0f, actual.y);
    CLOVE_FLOAT_EQ(20.0f, actual.width);
    CLOVE_FLOAT_EQ(10.0f, actual.height);
}
