/*
 * This is an example of possible way to implements polymorphic objects in C
 * while sharing the set of function pointers between all objects of the
 * same type.
 */

// So we can use strdup
#define _POSIX_C_SOURCE 200809L

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// (Cfr our discussion during class about not duplicating the function
// pointers for each object of the same type)


struct figure_object_t;
typedef struct figure_object_t figure_object_t;

typedef struct
{
    void (*print_name) (figure_object_t *);
    unsigned int (*area) (figure_object_t *);
    void (*destroy) (figure_object_t *);
} figure_vtable_t;

struct figure_object_t
{
    figure_vtable_t * vtable;
};

static void figure_print_name(figure_object_t * f)
{
    f->vtable->print_name(f);
}

static unsigned int figure_area(figure_object_t * f)
{
    return f->vtable->area(f);
}

static void figure_destroy(figure_object_t * f)
{
    f->vtable->destroy(f);
    free(f);
}

// ----------------------- rectangle -------------------

typedef struct
{
    figure_object_t figure;
    unsigned int width;
    unsigned int height;
} rectangle_object_t;

static unsigned int rectangle_area(figure_object_t * f)
{
    // We know that our figure_object_t pointer is really a pointer
    // to a rectangle_object_t.
    rectangle_object_t * this = (rectangle_object_t *) f;
    return this->width * this->height;
}

static void rectangle_destroy(figure_object_t * f)
{
    // nothing to do here; There is nothing to cleanup in a rectangle_object_t
    // (in addition to the figure_object_t cleanup)
}

static void rectangle_print_name(figure_object_t * f)
{
    rectangle_object_t * this = (rectangle_object_t *) f;
    fprintf(stdout, "rectangle(w=%u,h=%u)", this->width, this->height);
}

static figure_vtable_t rectangle_vtable = 
{
    .print_name = rectangle_print_name,
    .area = rectangle_area,
    .destroy = rectangle_destroy
};

rectangle_object_t * rectangle_create(unsigned int width, unsigned int height)
{
    // Note: we allocate enough space for a rectangle_object_t
    // But since the first member of the rectangle_object_t is a 
    // figure_object_t (and there is no padding at the start of a struct),
    // this means that both a rectangle_object_t and figure_object_t
    // structure start with the same thing: a pointer to figure_vtable_t.
    //
    // (This is really a consequence of the fact that every rectangle *is*
    // a figure, and the memory layout reflects this)
    //
    // All this means we can allocate a rectangle_object_t, but all
    // code outside of the rectangle_xxx functions can pretend
    // that the rectangle_object_t is really a figure_object_t.
    // (which it is)
    rectangle_object_t * ret = malloc(sizeof(*ret));
    ret->figure.vtable = &rectangle_vtable;
    ret->width = width;
    ret->height = height;
    return ret;
}

// --------------------- square ------------------------

typedef struct
{
    figure_object_t figure;
    unsigned int side;
    char * name;
} square_object_t;

void square_print_name(figure_object_t * f)
{
    square_object_t * this = (square_object_t *) f;
    fprintf(stdout, "square(name=%s,side=%u)", this->name, this->side);
}

void square_destroy(figure_object_t * f)
{
    square_object_t * this = (square_object_t *) f;
    // A square has some extra resources, in particular the dynamic
    // memory allocated in square's constructor.
    //
    // We need to free it here.
    free(this->name);
}

unsigned int square_area(figure_object_t * f)
{
    square_object_t * this = (square_object_t *) f;
    return this->side * this->side;
}

static figure_vtable_t square_vtable =
{
    .destroy= square_destroy,
    .area = square_area,
    .print_name = square_print_name
};

// Create a square object with the given side and name
square_object_t * square_create(unsigned int side, const char * name)
{
    square_object_t * obj = malloc(sizeof(*obj));
    obj->figure.vtable = &square_vtable;
    obj->side = side;
    obj->name = strdup(name);   // strdup mallocs memory...
    return obj;
}

// It is also possible to have functions specific to a concrete type
void square_specific(square_object_t * square)
{
    // do something
}

// ----- example usage ---


int main (int argc, char ** args)
{
    // Some evidence of the memory savings

    printf("size of rectangle object = %lu, size of square object = %lu,"
            " size of figure vtable = %lu\n",
            sizeof(rectangle_object_t), sizeof(square_object_t),
            sizeof(figure_vtable_t));

    // Example usage

    square_object_t * example_square = square_create(10, "Example");

    // Unfortunately we need to cast since the C type system has no concept
    // of objects, and so does not recognize that every rectangle_object_t
    // is really also a figure_object_t, and thus every rectangle_object_t *
    // pointer should implicitly be able to cast to a figure_object_t *.
    figure_object_t * objects[] = {
        (figure_object_t *) square_create(10, "square1"),
        (figure_object_t *) square_create(20, "square2"),
        (figure_object_t *) rectangle_create(30, 40),
        (figure_object_t *) square_create(100, "square3"),
        (figure_object_t *) example_square};

    const unsigned int count = sizeof(objects)/sizeof(objects[0]);

    // Show that we can use these objects in a polymorphic manner
    // (i.e. we don't really know what the concrete type for each fX is)

    for (unsigned int i=0u; i<count; ++i)
    {
        printf("%i: ", i);
        figure_print_name(objects[i]);
        printf(" area=%u\n", figure_area(objects[i]));
    }

    // Since we know example_square is a square, it's OK to call functions
    // which need a square.
    square_specific(example_square);

    // We need to clean them all up (note example_square is destroyed
    // here as well since it is in the array)
    for (unsigned int i=0u; i<count; ++i)
        figure_destroy(objects[i]);


}
