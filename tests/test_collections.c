#define CLOVE_SUITE_NAME CollectionsSuite
#include "clove-unit.h"

#include "aiv_list.h"
#include "aiv_vector.h"

CLOVE_TEST(VectorAddsAndReadsItems)
{
    int first = 10;
    int second = 20;
    aiv_vector_t vector = aiv_vector_new();

    aiv_vector_add(&vector, &first);
    aiv_vector_add(&vector, &second);

    CLOVE_SIZET_EQ(2, vector.count);
    CLOVE_PTR_EQ(&first, aiv_vector_at(&vector, 0));
    CLOVE_PTR_EQ(&second, aiv_vector_at(&vector, 1));
    CLOVE_NULL(aiv_vector_at(&vector, 2));

    aiv_vector_destroy(&vector);
}

CLOVE_TEST(VectorGrowsBeyondInitialCapacity)
{
    int values[16] = {0};
    aiv_vector_t vector = aiv_vector_new();

    for (size_t i = 0; i < 16; ++i) {
        values[i] = (int)i;
        aiv_vector_add(&vector, &values[i]);
    }

    CLOVE_SIZET_EQ(16, vector.count);
    CLOVE_INT_EQ(15, *(int*)aiv_vector_at(&vector, 15));

    aiv_vector_destroy(&vector);
}

CLOVE_TEST(VectorCanBeClearedByResettingCount)
{
    int first = 10;
    int second = 20;
    aiv_vector_t vector = aiv_vector_new();

    aiv_vector_add(&vector, &first);
    aiv_vector_add(&vector, &second);
    vector.count = 0;

    CLOVE_SIZET_EQ(0, vector.count);
    CLOVE_NULL(aiv_vector_at(&vector, 0));

    aiv_vector_destroy(&vector);
}

CLOVE_TEST(VectorDestroyReleasesStorage)
{
    aiv_vector_t vector = aiv_vector_new();
    aiv_vector_destroy(&vector);

    CLOVE_NULL(vector.items);
    CLOVE_SIZET_EQ(0, vector.count);
    CLOVE_SIZET_EQ(0, vector.capacity);
}

CLOVE_TEST(ListAddsItemsAndTracksCount)
{
    int first = 10;
    int second = 20;
    aiv_list_t list = aiv_list_new();

    CLOVE_IS_TRUE(aiv_list_is_empty(&list));

    aiv_list_add(&list, &first);
    aiv_list_add(&list, &second);

    CLOVE_IS_FALSE(aiv_list_is_empty(&list));
    CLOVE_SIZET_EQ(2, list.count);
    CLOVE_PTR_EQ(&first, list.head->data);
    CLOVE_PTR_EQ(&second, list.tail->data);

    aiv_list_destroy(&list);
}
