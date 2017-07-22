#ifndef __DEFINES_H__
#define __DEFINES_H__

#define CHECK(input, inputAgainst, output) if(inputAgainst == input) \
                              { \
                              return output; \
                              }
#define CHECK_RETURN_VOID(input, inputAgainst) if(inputAgainst == input) \
                              { \
                              return; \
                              }
#define RETURN_IF_EQUAL(check, checkAgainst, returnVal) if(checkAgainst == check) \
                              { \
                              return returnVal; \
                              }
#define RETURN_VOID_IF_EQUAL(check, checkAgainst) if(checkAgainst == check) \
                              { \
                              return; \
                              }

#define RELEASE_MEMORY_AND_RETURN_IF_EQUAL(check, checkAgainst, toFree, returnVal) if(checkAgainst == check) \
                                    {\
                                      free(toFree); \
                                      return returnVal; \
                                    }

#endif /* __DEFINES_H__ */
