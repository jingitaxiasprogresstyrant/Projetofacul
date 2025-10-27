/*
 * Sistema de Pedidos de Pizza
 * Uma aplicação de pedidos de pizza baseada em console
 * 
 * Este programa permite aos usuários pedir pizzas de um cardápio,
 * calcula o total com impostos e desconto opcional,
 * e salva os detalhes do pedido em um arquivo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/functions.h"

int main() {
    Pizza pizzas[MAX_PIZZAS] = {
        {"Pizza Portuguesa", 45.90},
        {"Pizza Francatu", 52.90},
        {"Pizza Mussarela", 42.90},
        {"Pizza Brocoli", 48.90},
        {"Pizza Calabresa", 46.90}
    };
    
    Order order = {0};
    int choice;
    int quantity;
    char continueOrdering;
    
    displayWelcomeBanner();
    
    printf("\n");
    displayMenu(pizzas, MAX_PIZZAS);
    
    do {
        printf("\n");
        choice = getValidInteger("Digite o número da pizza (1-5, 0 para finalizar): ", 0, MAX_PIZZAS);
        
        if (choice == 0) {
            if (order.itemCount == 0) {
                printf("\nVocê ainda não pediu nada. Tem certeza que deseja sair? (s/n): ");
                char confirm;
                scanf(" %c", &confirm);
                clearInputBuffer();
                if (confirm == 's' || confirm == 'S') {
                    break;
                } else {
                    continue;
                }
            }
            break;
        }
        
        quantity = getValidInteger("Digite a quantidade: ", 1, 99);
        
        addToOrder(&order, choice - 1, quantity, pizzas);
        printf("\n%d x %s adicionado ao seu pedido!\n", quantity, pizzas[choice - 1].name);
        
        if (order.itemCount < MAX_ORDER_ITEMS) {
            printf("\nGostaria de adicionar outra pizza? (s/n): ");
            scanf(" %c", &continueOrdering);
            clearInputBuffer();
            
            if (continueOrdering != 's' && continueOrdering != 'S') {
                break;
            }
        } else {
            printf("\nNúmero máximo de itens atingido.\n");
            break;
        }
        
    } while (1);
    
    if (order.itemCount == 0) {
        printf("\nNenhum item pedido. Obrigado pela visita!\n");
        return 0;
    }
    
    calculateOrderTotals(&order, pizzas);
    
    printf("\n");
    printf("===============================================\n");
    printf("             RESUMO DO PEDIDO\n");
    printf("===============================================\n");
    displayOrderSummary(&order, pizzas);
    
    saveOrderToFile(&order, pizzas);
    
    printf("\n===============================================\n");
    printf("  Obrigado pelo seu pedido! Bom apetite!\n");
    printf("===============================================\n\n");
    
    return 0;
}
