#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define MAX_PIZZAS 5
#define MAX_ORDER_ITEMS 20
#define TAX_RATE 0.10
#define DISCOUNT_THRESHOLD 150.0
#define DISCOUNT_RATE 0.10

typedef struct {
    char name[50];
    double price;
} Pizza;

typedef struct {
    int pizzaIndex;
    int quantity;
} OrderItem;

typedef struct {
    OrderItem items[MAX_ORDER_ITEMS];
    int itemCount;
    double subtotal;
    double tax;
    double discount;
    double total;
} Order;

void displayMenu(const Pizza pizzas[], int size);
void clearInputBuffer();
int getValidInteger(const char* prompt, int min, int max);
void addToOrder(Order* order, int pizzaIndex, int quantity, const Pizza pizzas[]);
void calculateOrderTotals(Order* order, const Pizza pizzas[]);
void displayOrderSummary(const Order* order, const Pizza pizzas[]);
void saveOrderToFile(const Order* order, const Pizza pizzas[]);
void displayWelcomeBanner();

#endif
