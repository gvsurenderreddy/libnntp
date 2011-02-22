#ifndef INTRUSIVE_PTR_H
#define INTRUSIVE_PTR_H 1

namespace boost
{
    template <typename T> inline void intrusive_ptr_add_ref(T *p)
    {
        // increment reference count of object *p
        ++(p->references);
    }

    template <typename T> inline void intrusive_ptr_release(T *p)
    {
        // decrement reference count
        --(p->references);

        // delete p when no more references exist
        if (p->references == 0)
            delete p;
    } 
}

#endif /* INTRUSIVE_PTR_H */
