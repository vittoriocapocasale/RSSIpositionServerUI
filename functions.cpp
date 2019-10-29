#include <functions.h>
#include <espserver.h>
uint64_t ntohll(uint64_t n)
{
    int num = 42;
    if (*(char *)&num == 42)
    {
        return (((uint64_t)ntohl(n)) << 32) + ntohl(n >> 32);
    }
    else
    {
        return n;
    }
}


uint64_t htonll(uint64_t n)
{
    int num = 42;
    if (*(char *)&num == 42)
    {
        return (((uint64_t)htonl(n)) << 32) + htonl(n >> 32);
    }
    else
    {
        return n;
    }
}

