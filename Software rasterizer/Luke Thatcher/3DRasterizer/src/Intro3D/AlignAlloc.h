
// Original Reference:
// http://webcache.googleusercontent.com/search?q=cache:pjjhCdBmUSAJ:ompf.org/forum/viewtopic.php%3Ff%3D11%26t%3D686

// The original reference has recently been removed from the internet. This reference has been found as the alternative:
// http://code.google.com/p/blademaster/source/browse/trunk/BladeMaster/Core/AlignedContainerAllocator.h?r=67

#pragma once

template <typename T, std::size_t N = 16>
class AlignAlloc : public allocator<T>
{
   public:
      typedef T value_type;
      typedef std::size_t size_type;
      typedef std::ptrdiff_t difference_type;

      typedef T * pointer;
      typedef const T * const_pointer;

      typedef T & reference;
      typedef const T & const_reference;

   public:
      inline AlignAlloc () throw ()
      {
      }

      template <typename T2>
      inline AlignAlloc (const AlignAlloc<T2, N> &) throw ()
      {
      }

      inline ~AlignAlloc () throw ()
      {
      }

      inline pointer adress (reference r)
      {
         return &r;
      }

      inline const_pointer adress (const_reference r) const
      {
         return &r;
      }

      inline pointer allocate (size_type n)
      {
         return (pointer)_mm_malloc (n*sizeof(value_type), N);
      }

      inline void deallocate (pointer p, size_type)
      {
         _mm_free (p);
      }

      inline void construct (pointer p, const value_type & wert)
      {
         new (p) value_type (wert);
      }

      inline void destroy (pointer p)
      {
         p->~value_type ();
      }

      inline size_type max_size () const throw ()
      {
         return size_type (-1) / sizeof (value_type);
      }

      template <typename T2>
      struct rebind
      {
         typedef AlignAlloc<T2, N> other;
      };
};