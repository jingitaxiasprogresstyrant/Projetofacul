/*
 * Implementação das funções do Sistema de Pedidos de Pizza
 * Contém toda a lógica de negócio para processamento de pedidos,
 * cálculos, validação de entrada e operações de arquivo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/functions.h"

/*
 * Exibir o banner de boas-vindas
 */
void displayWelcomeBanner() {
    printf("\n");
    printf("***********************************************\n");
    printf("*                                             *\n");
    printf("*     BEM-VINDO AO SISTEMA DE PEDIDOS DE     *\n");
    printf("*                   PIZZA                     *\n");
    printf("*                                             *\n");
    printf("***********************************************\n");
}

/*
 * Exibir o cardápio de pizzas com preços
 * Parâmetros:
 *   pizzas - array de structs Pizza
 *   size - número de pizzas no array
 */
void displayMenu(const Pizza pizzas[], int size) {
    printf("\n");
    printf("===============================================\n");
    printf("               CARDÁPIO DE PIZZAS\n");
    printf("===============================================\n");
    
    for (int i = 0; i < size; i++) {
        printf("%d. %-25s R$ %.2f\n", i + 1, pizzas[i].name, pizzas[i].price);
    }
    
    printf("===============================================\n");
}

/*
 * Limpar o buffer de entrada para evitar problemas com scanf
 */
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
 * Obter uma entrada inteira válida do usuário dentro de um intervalo especificado
 * Parâmetros:
 *   prompt - mensagem a exibir ao usuário
 *   min - valor mínimo aceitável
 *   max - valor máximo aceitável
 * Retorna:
 *   inteiro válido dentro do intervalo especificado
 */
int getValidInteger(const char* prompt, int min, int max) {
    int value;
    int result;
    
    while (1) {
        printf("%s", prompt);
        result = scanf("%d", &value);
        clearInputBuffer();
        
        if (result != 1) {
            printf("Entrada inválida! Por favor, digite um número.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Escolha inválida! Por favor, digite um número entre %d e %d.\n", min, max);
            continue;
        }
        
        return value;
    }
}

/*
 * Adicionar um item ao pedido
 * Parâmetros:
 *   order - ponteiro para a struct Order
 *   pizzaIndex - índice da pizza no array de pizzas
 *   quantity - número de pizzas a adicionar
 *   pizzas - array de pizzas disponíveis
 */
void addToOrder(Order* order, int pizzaIndex, int quantity, const Pizza pizzas[]) {
    int found = 0;
    
    for (int i = 0; i < order->itemCount; i++) {
        if (order->items[i].pizzaIndex == pizzaIndex) {
            order->items[i].quantity += quantity;
            found = 1;
            break;
        }
    }
    
    if (!found && order->itemCount < MAX_ORDER_ITEMS) {
        order->items[order->itemCount].pizzaIndex = pizzaIndex;
        order->items[order->itemCount].quantity = quantity;
        order->itemCount++;
    }
}

/*
 * Calcular os totais do pedido incluindo subtotal, imposto, desconto e total final
 * Parâmetros:
 *   order - ponteiro para a struct Order
 *   pizzas - array de pizzas disponíveis
 */
void calculateOrderTotals(Order* order, const Pizza pizzas[]) {
    order->subtotal = 0.0;
    
    for (int i = 0; i < order->itemCount; i++) {
        int pizzaIndex = order->items[i].pizzaIndex;
        int quantity = order->items[i].quantity;
        order->subtotal += pizzas[pizzaIndex].price * quantity;
    }
    
    order->discount = 0.0;
    if (order->subtotal > DISCOUNT_THRESHOLD) {
        order->discount = order->subtotal * DISCOUNT_RATE;
    }
    
    double subtotalAfterDiscount = order->subtotal - order->discount;
    order->tax = subtotalAfterDiscount * TAX_RATE;
    order->total = subtotalAfterDiscount + order->tax;
}

/*
 * Exibir o resumo completo do pedido
 * Parâmetros:
 *   order - ponteiro para a struct Order
 *   pizzas - array de pizzas disponíveis
 */
void displayOrderSummary(const Order* order, const Pizza pizzas[]) {
    printf("\nItens Pedidos:\n");
    printf("-----------------------------------------------\n");
    
    for (int i = 0; i < order->itemCount; i++) {
        int pizzaIndex = order->items[i].pizzaIndex;
        int quantity = order->items[i].quantity;
        double itemTotal = pizzas[pizzaIndex].price * quantity;
        
        printf("%d x %-25s R$ %.2f\n", 
               quantity, 
               pizzas[pizzaIndex].name, 
               itemTotal);
    }
    
    printf("-----------------------------------------------\n");
    printf("Subtotal:                          R$ %.2f\n", order->subtotal);
    
    if (order->discount > 0) {
        printf("Desconto (10%% em pedidos acima de R$ 150):  -R$ %.2f\n", order->discount);
    }
    
    printf("Taxas (10%%):                        R$ %.2f\n", order->tax);
    printf("-----------------------------------------------\n");
    printf("TOTAL:                             R$ %.2f\n", order->total);
}

/*
 * Salvar os detalhes do pedido em um arquivo de texto
 * Parâmetros:
 *   order - ponteiro para a struct Order
 *   pizzas - array de pizzas disponíveis
 */
void saveOrderToFile(const Order* order, const Pizza pizzas[]) {
    FILE* file = fopen("data/orders.txt", "a");
    
    if (file == NULL) {
        printf("\nAviso: Não foi possível salvar o pedido no arquivo.\n");
        return;
    }
    
    time_t now;
    time(&now);
    struct tm* timeinfo = localtime(&now);
    
    fprintf(file, "\n===============================================\n");
    fprintf(file, "Data do Pedido: %02d/%02d/%04d %02d:%02d:%02d\n",
            timeinfo->tm_mday,
            timeinfo->tm_mon + 1,
            timeinfo->tm_year + 1900,
            timeinfo->tm_hour,
            timeinfo->tm_min,
            timeinfo->tm_sec);
    fprintf(file, "===============================================\n");
    
    fprintf(file, "Itens:\n");
    for (int i = 0; i < order->itemCount; i++) {
        int pizzaIndex = order->items[i].pizzaIndex;
        int quantity = order->items[i].quantity;
        double itemTotal = pizzas[pizzaIndex].price * quantity;
        
        fprintf(file, "  %d x %-25s R$ %.2f\n", 
                quantity, 
                pizzas[pizzaIndex].name, 
                itemTotal);
    }
    
    fprintf(file, "-----------------------------------------------\n");
    fprintf(file, "Subtotal: R$ %.2f\n", order->subtotal);
    
    if (order->discount > 0) {
        fprintf(file, "Desconto: -R$ %.2f\n", order->discount);
    }
    
    fprintf(file, "Taxas:    R$ %.2f\n", order->tax);
    fprintf(file, "TOTAL:    R$ %.2f\n", order->total);
    fprintf(file, "===============================================\n\n");
    
    fclose(file);
    
    printf("\nPedido salvo em data/orders.txt\n");
}
