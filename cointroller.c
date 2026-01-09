#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//DEFINING THE COLOURS to make the UI interactive
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define WHITE   "\033[1;37m"
#define RESET   "\033[0m"
typedef struct
{
    char type[10];
    float amount;
    char category[30];
    char reason[100];
} Transaction;
Transaction list[1000];//it can store upto 1000 transactions
int count = 0;
float bud_hnu = 0;
float bud_food = 0;
float bud_trans = 0;
float bud_personal = 0;
float bud_edu = 0;
float bud_ent = 0;
void flush()//this function will help to prevent segmentation fault while using fgets
 {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        {}
}

void enter()
 {
    printf(YELLOW "\nPress the ENTER BUTTON to continue!" RESET);
    flush();
}
void save_b()
 {
    FILE *f = fopen("budgets.txt", "w");
    if (!f) return;
    fprintf(f, "%.2f %.2f %.2f %.2f %.2f %.2f\n",bud_hnu, bud_food, bud_trans, bud_personal, bud_edu, bud_ent);
    fclose(f);
}
void load_b()
 {
    FILE *f = fopen("budgets.txt", "r");
    if (!f) return;
    fscanf(f, "%f %f %f %f %f %f",&bud_hnu, &bud_food, &bud_trans,&bud_personal, &bud_edu, &bud_ent);
    fclose(f);
}
void save_t(Transaction *t)
 {
    FILE *f = fopen("transactions.txt", "a");
    if (!f) return;
    fprintf(f, "%s %.2f %s %s\n",t->type, t->amount, t->category, t->reason);
    fclose(f);
}
void save_all()
 {
    FILE *f = fopen("transactions.txt", "w");
    if (!f) return;
    for (int i = 0; i < count; ++i)
        fprintf(f, "%s %.2f %s %s\n",list[i].type, list[i].amount,list[i].category, list[i].reason);
    fclose(f);
}
void load_t()
 {
    FILE *f = fopen("transactions.txt", "r");
    if (!f) return;
    Transaction t;
    while (1)
    {
        int r = fscanf(f, "%9s %f %29s %99[^\n]",t.type, &t.amount, t.category, t.reason);
        if (r == EOF || r < 4)
        break;
        list[count++] = t;
    }
    fclose(f);
}
void title()
 {
    printf(CYAN "\n===========================================================\n" RESET);
    printf(CYAN "        COINTROLLER : LEVEL UP YOUR FINANCIAL GAME         \n" RESET);
    printf(CYAN "===========================================================\n\n" RESET);
}
void menu()
{
    title();
    printf(MAGENTA "| 1 | " RESET CYAN "Add Transaction\n" RESET);
    printf(MAGENTA "| 2 | " RESET CYAN "Edit Transaction\n" RESET);
    printf(MAGENTA "| 3 | " RESET CYAN "Delete Transaction\n" RESET);
    printf(MAGENTA "| 4 | " RESET CYAN "Search Transactions\n" RESET);
    printf(MAGENTA "| 5 | " RESET CYAN "View Summary + Charts\n" RESET);
    printf(MAGENTA "| 6 | " RESET CYAN "Set Category Budgets\n" RESET);
    printf(MAGENTA "| 7 | " RESET CYAN "Show All Transactions (Table)\n" RESET);
    printf(MAGENTA "| 8 | " RESET CYAN "Exit\n" RESET);
    printf(CYAN "-----------------------------------------------------------\n" RESET);
    printf("Enter choice: ");
}
void percent_bar(float current, float budget)
 {
    if (budget <= 0)
    {
        printf(YELLOW "[No budget set]\n" RESET);
        return;
    }
    float percent = (current / budget) * 100;
    if (percent > 100)
    percent = 100;
    int bars = percent / 5;
    printf("[");
    for (int i = 0; i < 20; i++)
    {
        if (i < bars)
        printf(GREEN "[.]" RESET);
        else printf("-");
    }
    printf("]  %.1f%%\n", (current / budget) * 100);
}
void add_transaction()
 {
    Transaction t;
    int ch;
    printf(CYAN "\n----- ADD TRANSACTION -----\n" RESET);
    printf("1) Deposit\n2) Withdraw\nChoose: ");
    scanf("%d", &ch);
    flush();
    if (ch == 1)
    strcpy(t.type, "deposit");
    else if (ch == 2)
    strcpy(t.type, "withdraw");
    else
    {
    printf(RED "Invalid\n" RESET);
    return;
    }
    printf("Enter amount: ");
    scanf("%f", &t.amount);
    flush();
    printf("\nCategory:\n");
    printf("1) Home & Utilities\n2) Food\n3) Transport\n4) Personal\n5) Education\n6) Entertainment\n");
    printf("Choose: ");
    scanf("%d", &ch);
    flush();
    switch (ch)
     {
        case 1:
        strcpy(t.category, "HomeUtilities");
        break;
        case 2:
        strcpy(t.category, "Food");
        break;
        case 3:
        strcpy(t.category, "Transport");
         break;
        case 4:
        strcpy(t.category, "Personal");
        break;
        case 5:
        strcpy(t.category, "Education");
        break;
        case 6:
        strcpy(t.category, "Entertainment");
        break;
        default:
        printf(RED "Invalid\n" RESET);
        return;
    }
    printf("Reason: ");
    fgets(t.reason, sizeof(t.reason), stdin);
    t.reason[strcspn(t.reason, "\n")] = 0;
    list[count++] = t;
    save_t(&t);
    printf(GREEN "\nTransaction Added!\n" RESET);
    enter();
}
void show_table()
 {
    title();
    printf(WHITE "%-4s %-10s %-10s %-15s %-40s\n" RESET,
           "No.", "Type", "Amount", "Category", "Reason");
    printf(CYAN "-------------------------------------------------------------------------------\n" RESET);
    for (int i = 0; i < count; ++i)
        {
        const char *col = strcmp(list[i].type, "deposit") == 0 ? GREEN : RED;
        printf("%-4d %s%-10s" RESET " %-10.2f %-15s %-40s\n",i + 1,col, list[i].type,list[i].amount,list[i].category,list[i].reason);
    }
    printf(CYAN "-------------------------------------------------------------------------------\n" RESET);
    enter();
}
void edit_transaction()
{
    if (count == 0)
    {
            printf(RED "No transactions.\n" RESET);
            return;
    }
    show_table();
    int idx;
    printf("Enter number to edit: ");
    scanf("%d", &idx);
    flush();
    idx--;
    if (idx < 0 || idx >= count)
    {
       printf(RED "Invalid\n" RESET);
        return;
    }
    printf("New amount (0 to skip): ");
    float amt;
    scanf("%f", &amt);
    flush();
    if (amt > 0)
        list[idx].amount = amt;
    printf("New reason (leave blank to skip): ");
    char buf[100];
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = 0;
    if (strlen(buf) > 0)
    {
        strcpy(list[idx].reason, buf);
    }
    save_all();
    printf(GREEN "Updated.\n" RESET);
    enter();
}
void delete_transaction()
 {
    if (count == 0)
    {
        printf(RED "Nothing to delete.\n" RESET);
        return;
    }
    show_table();
    int idx;
    printf("Enter number to delete: ");
    scanf("%d", &idx);
    flush();
    idx--;
    if (idx < 0 || idx >= count)
    {
        printf(RED "Invalid\n" RESET);
        return;
    }
    for (int i = idx; i < count - 1; i++)
    list[i] = list[i + 1];
    count--;
    save_all();
    printf(GREEN "Deleted.\n" RESET);
    enter();
}
void search_transactions()
 {
    if (count == 0)
    {
    printf(RED "No transactions.\n" RESET);
    return;
    }
    printf("\n1) Search by Reason\n2) Search by Category\nChoose: ");
    int ch;
    scanf("%d", &ch);
    flush();
    if (ch == 1)
    {
        char key[100];
        printf("Enter keyword: ");
        fgets(key, sizeof(key), stdin);
        key[strcspn(key, "\n")] = 0;
        int found = 0;
        printf(CYAN "\nResults:\n" RESET);
        for (int i = 0; i < count; ++i)
         {
            if (strstr(list[i].reason, key))
             {
                printf("%d) %s %.2f %s %s\n",i + 1,list[i].type,list[i].amount,list[i].category,list[i].reason);
                found = 1;
            }
        }
        if (!found) printf(YELLOW "No matches.\n" RESET);
    }
    else if (ch == 2)
    {
        char cat[30];
        printf("Enter category (Food/Transport/etc): ");
        fgets(cat, sizeof(cat), stdin);
        cat[strcspn(cat, "\n")] = 0;
        int found = 0;
        for (int i = 0; i < count; ++i)
         {
            if (strcasecmp(list[i].category, cat) == 0)
            {
                printf("%d) %s %.2f %s %s\n",i + 1,list[i].type,list[i].amount,list[i].category,list[i].reason);
                found = 1;
            }
        }
        if (!found) printf(YELLOW "Not found.\n" RESET);
    }
    enter();
}
void view_summary()
{
    float sum_hnu = 0;
    float sum_food = 0;
    float sum_trans = 0;
    float sum_personal = 0;
    float sum_edu = 0;
    float sum_ent = 0;
    float dep = 0;
    float wd = 0;
    for (int i = 0; i < count; i++)
        {
        if (strcmp(list[i].type, "deposit") == 0)
        dep += list[i].amount;
        else
        wd += list[i].amount;

        if (strcmp(list[i].category, "HomeUtilities") == 0)
            sum_hnu += list[i].amount;
        else if (strcmp(list[i].category, "Food") == 0)
            sum_food += list[i].amount;
        else if (strcmp(list[i].category, "Transport") == 0)
            sum_trans += list[i].amount;
        else if (strcmp(list[i].category, "Personal") == 0)
            sum_personal += list[i].amount;
        else if (strcmp(list[i].category, "Education") == 0)
            sum_edu += list[i].amount;
        else if (strcmp(list[i].category, "Entertainment") == 0)
            sum_ent += list[i].amount;
    }
    title();
    printf(WHITE "Total Deposit : %.2f\n" RESET, dep);
    printf(WHITE "Total Withdraw: %.2f\n" RESET, wd);
    printf(WHITE "Balance       : %.2f\n\n" RESET, dep - wd);
    printf(CYAN "----- CATEGORY CHART (Percentage Bars) -----\n" RESET);
    printf("\nHome & Utilities: ");
    percent_bar(sum_hnu, bud_hnu);
    printf("Food           : ");
    percent_bar(sum_food, bud_food);
    printf("Transport      : ");
    percent_bar(sum_trans, bud_trans);
    printf("Personal       : ");
    percent_bar(sum_personal, bud_personal);
    printf("Education      : ");
    percent_bar(sum_edu, bud_edu);
    printf("Entertainment  : ");
    percent_bar(sum_ent, bud_ent);
    enter();
}
void set_budgets()
 {
    printf("\nEnter budgets (0 = skip):\n");
    printf("Home & Utilities: ");
    scanf("%f", &bud_hnu);
    flush();
    printf("Food           : ");
    scanf("%f", &bud_food);
    flush();
    printf("Transport      : ");
    scanf("%f", &bud_trans);
    flush();
    printf("Personal       : ");
    scanf("%f", &bud_personal);
    flush();
    printf("Education      : ");
    scanf("%f", &bud_edu);
    flush();
    printf("Entertainment  : ");
    scanf("%f", &bud_ent);
    flush();
    save_b();
    printf(GREEN "Budgets saved.\n" RESET);
    enter();
}
int main()
{
    load_b();
    load_t();
    while (1)
    {
        menu();
        int choice;
        scanf("%d", &choice);
        flush();
        switch (choice)
         {
            case 1:
            add_transaction();
            break;
            case 2:
            edit_transaction();
            break;
            case 3:
            delete_transaction();
            break;
            case 4:
            search_transactions();
            break;
            case 5:
            view_summary();
            break;
            case 6:
            set_budgets();
            break;
            case 7:
            show_table();
            break;
            case 8:
            printf(GREEN "Goodbye!\n" RESET);
            return 0;
            default:
            printf(RED "Invalid option\n" RESET);
        }
    }
    return 0;
}


