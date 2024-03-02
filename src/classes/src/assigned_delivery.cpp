#include "assigned_delivery.h"

AssignedDelivery::AssignedDelivery(char* ad_delivery_code, char* ad_delivery_assignment_instant, char* ad_delivery_start, char* ad_delivery_end, char* ad_delivery_state, char* ad_lost_instant, char* ad_courier, char* ad_purchase) {
    delivery_code = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    delivery_assignment_instant = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    delivery_start = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    delivery_end = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    delivery_state = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    lost_instant = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    courier = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    purchase = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(delivery_code, ad_delivery_code);
    strcpy(delivery_assignment_instant, ad_delivery_assignment_instant);
    strcpy(delivery_start, ad_delivery_start);
    strcpy(delivery_end, ad_delivery_end);
    strcpy(delivery_state, ad_delivery_state);
    strcpy(lost_instant, ad_lost_instant);
    strcpy(courier, ad_courier);
    strcpy(purchase, ad_purchase);
}

AssignedDelivery::~AssignedDelivery() {
    free(delivery_code);
    free(delivery_assignment_instant);
    free(delivery_start);
    free(delivery_end);
    free(delivery_state);
    free(lost_instant);
    free(courier);
    free(purchase);
}
