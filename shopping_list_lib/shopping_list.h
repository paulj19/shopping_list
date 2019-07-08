#ifndef s_list
#define s_list

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
//defines the single item in the shopping list
using byte = unsigned char;
struct Item{
    char* name;
    int price;
    bool bought;
    byte* uid;
    Item* right;
    Item* left;
    Item(): name{nullptr}, price{0}, bought{false} , left{nullptr}, right{nullptr} {}
    Item(char *name_, int price_, bool bought_, Item* left_, Item* right_): name{name_}, price{price_}, bought{bought_}, left{left_}, right{right_} {}
//    bool operator==(Item other) price{price_}, bought{bought_};
};

//the shopping list implemented as list
class list_items{
    int size;
    Item* first_item;
    Item* last_item;

    public:

       class iterator;
       //Item* first_item_() {return first_item;} 
       //Item* last_item_() {return last_item;} 
       iterator begin();
       iterator end();

       list_items(): size{0}, first_item{nullptr}, last_item{nullptr} {}
       list_items(char*);
       void print_list();
       iterator next(iterator pos);
       iterator erase(iterator pos);
       void mark_as_bought(iterator pos);
       int get_item_price(iterator pos);
       bool isEmpty();
       void set_price(iterator it, int price);
       void set_uid(iterator it, byte* uid);
};

class list_items::iterator{
    Item* item;
    public:
        friend class list_items;
        iterator(): item{nullptr} {}
        iterator(Item *it): item{it} {}
        Item* operator->(){ if(this) return item; else return nullptr;}
        char* operator*()
        {
                return item->name;
        }
        bool operator==(list_items::iterator other)
        {
            if(item)
                if(other.item)
                    return !(strcmp((other.item)->name, (item)->name)); 
                else
                    return false;
                
            else    
                if(!other.item)
                    return true;
               return false;
        }
        bool operator!=(list_items::iterator other)
        { 
            if(item)
                if(other.item)
                    return (strcmp((other.item)->name, (item)->name)); 
                else
                    return false;
                
            else    
                if(other.item)
                    return true;
               return false;
        }
        bool isEnd()
        {
            if(item)
               return false; 
            else    
               return true;
        }
        void print()
        {
            printf("\n %s", item->name);
            printf("\n %d", item->price);
            printf("\n %d", item->bought);
            printf("\n %x", (item->uid)[0]);
            printf("\n %x", (item->uid)[1]);
            printf("\n %x", (item->uid)[2]);
            printf("\n %x", (item->uid)[2]);
        }
};

//char* get_price(char*);
//char* get_name(char*);
#endif
