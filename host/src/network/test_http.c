#include <host/network.h>
#include <stdio.h>

#define BUF_SIZE 1024

int main(void) {
    /* Test for fetch failure*/
    struct net_device http_dev_fail = {0};
    
    if (net_dev_init(&http_dev_fail, 
                     &http_ops, 
                     "fakewebsite.invalid", 
                     "/fake") < 0) 
    {
        printf("Failed to init dev_fail\n\n");
        return 1;
    }

    char buf[BUF_SIZE] = {0};
    if (net_dev_fetch(&http_dev_fail, buf, BUF_SIZE) < 0)
        printf("Failed to fetch dev_fail\n");
    else
        printf("From dev_fail: %s\n", buf);
    
    /* Test for fetch success */
    struct net_device http_dev_success = {0};
    if (net_dev_init(&http_dev_success, 
                     &http_ops, 
                     "httpbin.org", 
                     "/post") < 0) 
    {
        printf("Failed to init dev_success\n");
        return 1;
    }

    if (net_dev_fetch(&http_dev_success, buf, BUF_SIZE) < 0) {
        printf("Failed to fetch dev_success\n");
        return 1;
    }

    printf("From dev_success: %s\n", buf);

    return 0;
}
