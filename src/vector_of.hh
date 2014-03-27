/**
 * @vector_of.hh
 */
#ifndef _VECTOR_OF_HH_
#define _VECTOR_OF_HH_

#include <vector>

/*
    virtual void push_back(SINGULAR##Interface* abstract) = 0;           \
    virtual void erase(vector<SINGULAR*>::iterator position) = 0;        \
    virtual size_t size() const = 0;                                     \
                                                                         \
                                                                         \
                virtual public PLURAL##Interface {                       \
*/
#define VECTOR_OF(SINGULAR, PLURAL, MAX_ENTRIES)                          \
  using namespace std;                                                    \
                                                                          \
  class PLURAL##Interface {                                               \
                                                                          \
  public:                                                                 \
                                                                          \
    PLURAL##Interface() {};                                               \
    virtual ~PLURAL##Interface() {};                                      \
                                                                          \
  };                                                                      \
                                                                          \
  class PLURAL: virtual public PLURAL##Interface,                         \
                                     virtual public vector<SINGULAR*> {   \
  protected:                                                              \
                                                                          \
    unsigned int current_index;                                           \
                                                                          \
  public:                                                                 \
                                                                          \
    static const size_t MAX = (MAX_ENTRIES);                              \
                                                                          \
    PLURAL() : vector<SINGULAR*>(), current_index(0) {}                   \
    virtual ~PLURAL() {}                                                  \
  }

#endif
