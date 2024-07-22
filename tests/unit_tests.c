#define TEST_LIST

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../cc.h"
#include "../Unity-master/src/unity.h"

// Custom realloc and free functions that track the number of outstanding allocations.
// If SIMULATE_ALLOC_FAILURES is defined above, the realloc will also sporadically fail.

size_t simulated_alloc_failures = 0;
size_t oustanding_allocs = 0;

void *unreliable_tracking_realloc( void *ptr, size_t size )
{
#ifdef SIMULATE_ALLOC_FAILURES
  if( rand() % 5 == 0 )
  {
    ++simulated_alloc_failures;
    return NULL;
  }
#endif

  void *new_ptr = realloc( ptr, size );
  TEST_ASSERT_NOT_NULL_MESSAGE( new_ptr, "Realloc failed to allocate pointer in unreliable_tracking_realloc()" );

  if( !ptr )
    ++oustanding_allocs;

  return new_ptr;
}
void tracking_free( void *ptr )
{
  if( ptr )
    --oustanding_allocs;

  free( ptr );
}
// Activate custom realloc and free functions.
#define CC_REALLOC unreliable_tracking_realloc
#define CC_FREE tracking_free
// Define a custom type that will be used to check that destructors are always called where necessary.
bool dtor_called[ 100 ];
void check_dtors_arr( void )
{
  for( size_t i = 0; i < sizeof( dtor_called ) / sizeof( *dtor_called ); ++i )
  {
    TEST_ASSERT_TRUE_MESSAGE( dtor_called[ i ], "dtor_called boolean array inside check_dtors_arr() was not initialized to all true values");
    dtor_called[ i ] = false;
  }
}
typedef struct { int val; } custom_ty;
#define CC_DTOR custom_ty, { dtor_called[ val.val ] = true; }
#define CC_CMPR custom_ty, { return val_1.val < val_2.val ? -1 : val_1.val > val_2.val; }
#define CC_HASH custom_ty, { return val.val * 2654435761ull; }
#define CC_LOAD custom_ty, 0.7
#include "../cc.h"












































list( int ) our_list;
list( int ) src_list;

void setUp(void)
{
  // This function will be run before each test
  init( &our_list );
  init( &src_list );
}

void tearDown(void)
{
  // This function will be run after each test
  cleanup( &our_list );
  cleanup( &src_list );
}

// List tests.
#ifdef TEST_LIST

#define LIST_CHECK                                                              \
TEST_ASSERT_EQUAL_MESSAGE( size( &our_list ), sizeof( expected ) / sizeof( *expected ), "Size of list is not the expected"); \
do                                                                              \
{                                                                               \
  int *arr = expected;                                                          \
  for_each( &our_list, i )                                                      \
  {                                                                             \
    TEST_ASSERT_EQUAL_MESSAGE( *i, *arr, "pointer to our list is not the expected");                                                \
    ++arr;                                                                      \
  }                                                                             \
}while( false )                                                                 \

void unity_translated_test_list_insert( void )
{
  int expected [ 90 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89
  };

  // End.
  for( int i = 0; i < 30; ++i )
  {
    int *el;
    el = insert( &our_list, end( &our_list ), 60 + i);
    TEST_ASSERT_NOT_NULL_MESSAGE(el, "Realloc failed to insert");
    TEST_ASSERT_EQUAL_MESSAGE( *el,60 + i , "Insert operation value not expected");
  }

  // Beginning.
  for( int i = 0; i < 30; ++i )
  {
    int *el;
    el = insert( &our_list, first( &our_list ), 29 - i );
    TEST_ASSERT_NOT_NULL_MESSAGE(el, "Realloc failed to insert");
    TEST_ASSERT_EQUAL_MESSAGE( *el, 29 - i, "Insert operation value not expected");
  }

  // Middle.
  int *mid = first( &our_list );
  for( int i = 0; i < 30; ++i )
    mid = next( &our_list, mid );

  for( int i = 0; i < 30; ++i )
  {
    int *el;
    el = insert( &our_list, mid, 30 + i );
    TEST_ASSERT_NOT_NULL_MESSAGE(el, "Realloc failed to insert");
    TEST_ASSERT_EQUAL_MESSAGE( *el, 30 + i, "Insert operation value not expected");
  }

  LIST_CHECK;
}
/*
void unity_translated_test_list_push( void )
{
  int expected [ 100 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
    90, 91, 92, 93, 94, 95, 96, 97, 98, 99
  };

  for( int i = 0; i < 100; ++i )
  {
    int *el;
    UNTIL_SUCCESS( el = push( &our_list, i ) );
    ALWAYS_ASSERT( ( *el == i ) );
  }

  LIST_CHECK;

}

void unity_translated_test_list_splice( void )
{
  // Splice from one list to another.

  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( push( &src_list, i ) );

  bool splice = true;
  for( int *i = first( &src_list ); i != last( &src_list ); )
  {
    int *next = next( &src_list, i );

    if( splice )
      UNTIL_SUCCESS( splice( &our_list, end( &our_list ), &src_list, i ) );

    splice = !splice;
    i = next;
  }

  ALWAYS_ASSERT( size( &our_list ) == 50 );
  for( int *i = first( &our_list ), j = 0; i != last( &our_list ); i = next( &our_list, i ), j += 2 )
    ALWAYS_ASSERT( *i == j );

  ALWAYS_ASSERT( size( &src_list ) == 50 );
  for( int *i = first( &src_list ), j = 1; i != last( &src_list ); i = next( &src_list, i ), j += 2 )
    ALWAYS_ASSERT( *i == j );

  // Splice within the same list.

  int *i = last( &our_list );
  while( i != r_end( &our_list ) )
  {
    int *prev = prev( &our_list, i );

    UNTIL_SUCCESS( splice( &our_list, end( &our_list ), &our_list, i ) );

    i = prev;
  }

  ALWAYS_ASSERT( size( &our_list ) == 50 );
  for( int *i = first( &our_list ), j = 98; i != last( &our_list ); i = next( &our_list, i ), j -= 2 )
    ALWAYS_ASSERT( *i == j );

}

void unity_translated_test_list_erase( void )
{
  int expected [ 50 ] = {
    1, 3, 5, 7, 9,
    11, 13, 15, 17, 19,
    21, 23, 25, 27, 29,
    31, 33, 35, 37, 39,
    41, 43, 45, 47, 49,
    51, 53, 55, 57, 59,
    61, 63, 65, 67, 69,
    71, 73, 75, 77, 79,
    81, 83, 85, 87, 89,
    91, 93, 95, 97, 99
  };

  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( push( &our_list, i ) );

  bool erase = true;
  for( int *i = first( &our_list ); i != end( &our_list ); )
  {
    if( erase )
      i = erase( &our_list, i );
    else
      i = next( &our_list, i );

    erase = !erase;
  }

  LIST_CHECK;
}

void unity_translated_test_list_clear( void )
{

  int expected [ 30 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29
  };

  // Empty.
  clear( &our_list );
  ALWAYS_ASSERT( size( &our_list ) == 0 );

  // Non-empty.
  for( int i = 0; i < 30; ++i )
    UNTIL_SUCCESS( push( &our_list, i ) );
  ALWAYS_ASSERT( size( &our_list ) == 30 );

  clear( &our_list );
  ALWAYS_ASSERT( size( &our_list ) == 0 );

  // Test use.
  for( int i = 0; i < 30; ++i )
    UNTIL_SUCCESS( push( &our_list, i ) );
  LIST_CHECK;
}

void unity_translated_test_list_cleanup( void )
{
  int expected [ 30 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29
  };

  // Empty.
  cleanup( &our_list );
  ALWAYS_ASSERT( (void *)our_list == (void *)&cc_list_placeholder );

  // Non-empty.
  for( int i = 0; i < 30; ++i )
    UNTIL_SUCCESS( push( &our_list, i ) );
  ALWAYS_ASSERT( size( &our_list ) == 30 );
  cleanup( &our_list );
  ALWAYS_ASSERT( size( &our_list ) == 0 );
  ALWAYS_ASSERT( (void *)our_list == (void *)&cc_list_placeholder );

  // Test use.
  for( int i = 0; i < 30; ++i )
    UNTIL_SUCCESS( push( &our_list, i ) );
  LIST_CHECK;
}

// This needs to test, in particular, that r_end and end iterator-pointers are stable, especially between transition
// from placeholder to non-placeholder.
void unity_translated_test_list_iteration( void )
{
  int expected [ 30 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29
  };

  int *r_end = r_end( &our_list );
  int *end = end( &our_list );

  // Empty.

  // Test fist and last.
  ALWAYS_ASSERT( first( &our_list ) == end );
  ALWAYS_ASSERT( last( &our_list ) == r_end );

  int n_iterations = 0;
  for( int *i = first( &our_list ); i != end( &our_list ); i = next( &our_list, i ) )
    ++n_iterations;
  for( int *i = last( &our_list ); i != r_end( &our_list ); i = prev( &our_list, i ) )
    ++n_iterations;

  for_each( &our_list, i )
    ++n_iterations;
  for_each( &our_list, i )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 0 );

  // Non-empty.
  for( int i = 0; i < 30; ++i )
    UNTIL_SUCCESS( push( &our_list, i ) );
  
  for( int *i = first( &our_list ); i != end( &our_list ); i = next( &our_list, i ) )
    ++n_iterations;
  for( int *i = last( &our_list ); i != r_end( &our_list ); i = prev( &our_list, i ) )
    ++n_iterations;

  for_each( &our_list, i )
    ++n_iterations;
  for_each( &our_list, i )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 120 );

  // Test iterator stability.
  ALWAYS_ASSERT( r_end( &our_list ) == r_end );
  ALWAYS_ASSERT( end( &our_list ) == end );

  // Test fist and last.
  ALWAYS_ASSERT( *first( &our_list ) == 0 );
  ALWAYS_ASSERT( *last( &our_list ) == 29 );

  LIST_CHECK;
}

void unity_translated_test_list_init_clone( void )
{
  int expected [ 10 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9
  };

  // Test init_clone placeholder.
  list( int ) empty_list;
  UNTIL_SUCCESS( init_clone( &empty_list, &src_list ) );
  ALWAYS_ASSERT( (void *)empty_list == (void *)&cc_list_placeholder );

  // Test init_clone non-placeholder.
  list( int ) our_list;
  for( int i = 0; i < 10; ++i )
    UNTIL_SUCCESS( push( &src_list, i ) );
  UNTIL_SUCCESS( init_clone( &our_list, &src_list ) ); // Note that because elements are allocated individually,
                                                       // this loop may never exist if realloc failure rate is set too
                                                       // high.
  LIST_CHECK;
  cleanup( &empty_list );
}

void unity_translated_test_list_dtors( void )
{
  list( custom_ty ) our_list2;
  init( &our_list2 );

  // Test erase and clear.

  for( int i = 0; i < 100; ++i )
  {
    custom_ty el = { i };
    UNTIL_SUCCESS( push( &our_list2, el ) );
  }

  bool erase = true;
  for( custom_ty *i = first( &our_list2 ); i != end( &our_list2 ); )
  {
    if( erase )
      i = erase( &our_list2, i );
    else
      i = next( &our_list2, i );

    erase = !erase;
  }

  clear( &our_list2 );
  check_dtors_arr();

  // Test cleanup.

  for( int i = 0; i < 100; ++i )
  {
    custom_ty el = { i };
    UNTIL_SUCCESS( push( &our_list2, el ) );
  }

  cleanup( &our_list2 );
  check_dtors_arr();
}
*/
#endif

int main( void )
{
  srand( (unsigned int)time( NULL ) );

  // Repeat 1000 times since realloc failures are random.
  for( int i = 0; i < 10; ++i )
  {
    #ifdef TEST_LIST
    // list, init, and size are tested implicitly.
    RUN_TEST(unity_translated_test_list_insert);
    // RUN_TEST(unity_translated_test_list_push);
    // RUN_TEST(unity_translated_test_list_splice);
    // RUN_TEST(unity_translated_test_list_erase);
    // RUN_TEST(unity_translated_test_list_clear);
    // RUN_TEST(unity_translated_test_list_cleanup);
    // RUN_TEST(unity_translated_test_list_iteration);
    // RUN_TEST(unity_translated_test_list_init_clone);
    // RUN_TEST(unity_translated_test_list_dtors);
    #endif
  }

  TEST_ASSERT_EQUAL_size_t_MESSAGE(oustanding_allocs, 0, "There are still outstanding allocs");
  printf( "All done.\n" );
  printf( "Simulated realloc failures: %zu\n", simulated_alloc_failures );
}
