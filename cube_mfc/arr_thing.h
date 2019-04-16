//
// ARR_THING.H
// Array of THINGs class.
//
// Somewhat mimics MFCs CArray object, except that it works for large 
// numbers of things also.
//
// I doubt anyone checks the return value from things like array Add(), etc
// This and any other functions that internally call Realloc() with throw 
// a memory exception in addition to returning an error, if there's a 
// problem
//
// This software is licensed/distributed under the GPL 
// WITHOUT ANY WARRANTY - see gpl.license.txt for details
// Written by Ross Wolin/Mystic Industries - Copyright 2003
//


#ifndef ARR_THING_H
#define ARR_THING_H

#include <malloc.h>




template<class THING> class WThingArray 
{ 
protected:
   int m_nInitialSize;     //Initial space in the array (can fill up this much space without growing.)
   int m_nGrowBySize;      //Size of additional space to allocate when growing the array
   int m_ctThings;         //Number of things in the array
   THING* m_pThing;        //Chunk of alloc'd memory that stores the dynamic array

public:
   WThingArray(int nInitialElements=32, int nGrowBy=32); //Don't override these here!  Use SetSize() afterward
   ~WThingArray();                                       // if you want to change these, and be sure to check return

   BOOL SetSize(int ctElements, int nGrowBy);            //Sets dynamic memory allocation parameters
   
   BOOL Add(THING thing);
   BOOL Add(THING* pThing);
   THING operator [](int x)   {return GetAt(x);};
   THING GetAt(int x);      
   THING GetLast(void);
   THING& ElementAt(int x);

   int  GetSize(void)         {return m_ctThings;};      //Number of things, not allocated space size
   BOOL IsEmpty(void)         {return m_ctThings==0;};

   void RemoveAll(void)       {m_ctThings = 0;};
   void RemoveLast(void);
   BOOL FreeExtra(void);

protected:
   BOOL Realloc(int nNewSize);
};    





//Don't override these parameters here, unless you REALLY want to....
//Instead use SetSize() afterward if you want to change these, and be sure to check return value 
//from SetSize()   (If you trigger a memory allocation error in the constructor by changing these
//size, I can't do anything except throw a memory exception... this could be okay, depending on 
//how the caller handles it.
template<class THING> WThingArray<THING>::WThingArray(int nInitialElements/*=32*/, int nGrowBy/*=32*/)
: m_ctThings(0), m_pThing(NULL)
{
   m_nInitialSize = nInitialElements * sizeof(THING);
   m_nGrowBySize = nGrowBy * sizeof(THING);

   m_pThing=(THING*)malloc(m_nInitialSize);

   //HACK - this probably needs more intelligent handling, although I
   //would hope an alloc of 32 would never fail.
   if (!m_pThing)
      AfxThrowMemoryException();
}



template<class THING> WThingArray<THING>::~WThingArray()
{
   if (m_pThing) {
      free(m_pThing);
      m_pThing = NULL;
   }
}



//I doubt anyone checks the return value from an array Add.
//This and any other function that calls Realloc with throw a memory exception
//in addition to returning an error
template<class THING> BOOL WThingArray<THING>::Add(THING t)
{
   ASSERT(m_pThing);

   //Check if we need to alloc more space
   int nSize=(int)_msize(m_pThing);
   if (nSize < (int)sizeof(THING)*(m_ctThings+1)) {
      if (!Realloc(nSize + m_nGrowBySize))
         return FALSE;        //Check if realloc failed
   }

   m_pThing[m_ctThings++] = t;
   return TRUE;
}


template<class THING> BOOL WThingArray<THING>::Add(THING* p)
{
   ASSERT(m_pThing);

   //Check if we need to alloc more space
   int nSize=(int)_msize(m_pThing);
   if (nSize < (int)sizeof(THING)*(m_ctThings+1)) {
      if (!Realloc(nSize + m_nGrowBySize))
         return FALSE;        //Check if realloc failed
   }

   m_pThing[m_ctThings++] = *p;     //Make a copy using the copy constructor
   return TRUE;
}




//Free any extra allocated space, in increments of m_nInitialSize + N*m_nGrowBySize
template<class THING> BOOL WThingArray<THING>::FreeExtra(void)
{
   ASSERT(m_pThing);

   int nSize = (int)_msize(m_pThing);
   int nNeededSize = m_ctThings*(int)sizeof(THING);

   ASSERT(nSize >= m_nInitialSize);


   //Check if already alloc'd as small as we can go
   if (nSize <= m_nInitialSize)  
      return TRUE;

   //If we're here, we're bigger than initial size.. check if we need to be
   if (nNeededSize < m_nInitialSize) 
      return Realloc(m_nInitialSize);

   //If we've got more growby chunks than we need, eliminate the extras
   if (nSize - nNeededSize > m_nGrowBySize) {
      return Realloc(m_nInitialSize    //RSW - Someone should verify this one... I think this is right.
                     + (((nNeededSize - m_nInitialSize)/m_nGrowBySize + 1) * m_nGrowBySize));
   }

   return TRUE;
}



template<class THING> inline THING& WThingArray<THING>::ElementAt(int nIndex)
{
   ASSERT(m_pThing);
   ASSERT(nIndex < m_ctThings);

   return m_pThing[nIndex];
}



template<class THING> inline THING WThingArray<THING>::GetAt(int nIndex)
{
   ASSERT(m_pThing);
   ASSERT(nIndex < m_ctThings);

   return m_pThing[nIndex];
}



template<class THING> inline THING WThingArray<THING>::GetLast(void)
{
   return GetAt(m_ctThings-1);
}



template<class THING> inline void WThingArray<THING>::RemoveLast(void)
{
   if (m_ctThings>0)
      m_ctThings--;
}



//Sets dynamic memory allocation parameters
//Causes a realloc if the current size is smaller than the initial size,
//otherwise call FreeExtra if you wish to "trim" the array 
template<class THING> BOOL WThingArray<THING>::SetSize(int ctElements, int nGrowBy)
{
   m_nInitialSize = ctElements * sizeof(THING);
   m_nGrowBySize = nGrowBy * sizeof(THING);

   int nSize=(int)_msize(m_pThing);
   if (nSize < m_nInitialSize) 
      return Realloc(m_nInitialSize);

   return TRUE;
}



template<class THING> BOOL WThingArray<THING>::Realloc(int nNewSize)
{
   ASSERT(m_pThing);

   THING* p=(THING*)realloc(m_pThing, nNewSize);
   if (!p) {
      AfxThrowMemoryException();
      return FALSE;        //Check if realloc failed
   }

   m_pThing = p;
   return TRUE;
}

#endif //ARR_THING_H





