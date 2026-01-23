#include <host/network.h>
#include <stdio.h>

#define BUF_SIZE 1024
#define API_URL "/v1/forecast?latitude=39&longitude=-97"    \
                "&current=temperature_2m&forecast_days=1"   \

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

    /* Test for the real API */
    struct net_device http_dev_real = {0};
    if (net_dev_init(&http_dev_real,
                     &http_ops,
                     "api.open-meteo.com",
                     API_URL) < 0)
    {
        printf("Failed to init dev_real\n");
        return 1;
    }

    if (net_dev_fetch(&http_dev_real, buf, BUF_SIZE) < 0) {
        printf("Failed to fetch dev_real\n");
        return 1;
    }

    printf("From dev_real: %s\n", buf);

    return 0;
}
