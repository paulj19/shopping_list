#include "shopping_list.h"
list_items::iterator list_items::begin()
{
    return iterator(first_item);
}
list_items::iterator list_items::end() 
{
    return iterator(last_item);
}

/*
char* get_name(char* split)
{

    int split_len = strlen(split);

    int i = 0, j = 0;
    char temp[split_len];
    char temp_item_str[split_len];
    strcpy(temp, split);
    //printf("\n%s", temp);
   

    for(; temp[i] != ':' ; ++i)
    {
        temp_item_str[j++] = temp[i];
    //    printf("\n>>%c", temp_item_str[j-1]);
    }

    temp_item_str[j] = '\0';
     //   printf("\n$$%s", temp_item_str);

    char* item_name = (char*) malloc(sizeof(char) * strlen(temp_item_str));
    strcpy(item_name, temp_item_str);
    //printf("\n((%s))", item_name);

    return item_name;
}
char* get_price(char* split)
{
    int split_len = strlen(split);

    int i = 0, j = 0;
    char temp[split_len];
    char temp_price_str[split_len];
    strcpy(temp, split);
    //printf("\nin get price: %s", temp);
   
    while(temp[i++] != ':');

    for(; temp[i] != '\0' ; ++i)
    {
        temp_price_str[j++] = temp[i];
        //printf("\n<<%c", temp_price_str[j-1]);
    }

    temp_price_str[j] = '\0';
    //printf("\n&&%s", temp_price_str);

    char* price_str = (char*) malloc(sizeof(char) * strlen(temp_price_str));
    strcpy(price_str, temp_price_str);
    printf("\nprice str:%s", price_str);

    return price_str;
}
*/
list_items::list_items(char* init_string)
{
    char *split;
    Item* temp;
    Item* prev;
    Item temp_obj;

    if(init_string == nullptr) 
        return;
    last_item = nullptr;

    char* init_string_dup = strdup(init_string);

    split = strtok(init_string_dup, ",");
    char* split_dup = strdup(split);
    
    first_item = (Item *) malloc(sizeof(Item));
    //temp_obj = Item(item_name, atoi(price), false, nullptr, last_item);
    temp_obj = Item(split_dup, 0, false, nullptr, last_item);
    memcpy(first_item, &temp_obj, sizeof(Item));

    split = strtok(NULL , ",");
    split_dup = strdup(split);
    temp = (Item *) malloc(sizeof(Item));
    //temp_obj = Item(item_name,atoi(price), false, first_item, last_item);
    temp_obj = Item(split_dup ,0, false, first_item, last_item);
    memcpy(temp , &temp_obj, sizeof(Item));

    first_item->right = temp;
    prev = temp;

    split = strtok(NULL , ",");

    while(split != NULL){
        split_dup = strdup(split);
        temp = (Item *) malloc(sizeof(Item));
        //temp_obj = Item("a",10, false, item_name,atoi(price), false, prev, last_item);
        temp_obj = Item(split_dup, 0, false, prev, last_item);
        memcpy(temp , &temp_obj, sizeof(Item));
        prev->right = temp;
        prev = temp;
        split = strtok(NULL , ",");
    }

}
void list_items::print_list()
{
    Item *i = first_item;
    while(i != nullptr){
        printf("%s\n", i->name);
        i = i->right;
    }
}
void list_items::mark_as_bought(list_items::iterator pos)
{
    pos->bought = true;
}
int list_items::get_item_price(list_items::iterator pos)
{
    return pos->price;
}
list_items::iterator list_items::next(list_items::iterator pos)
{
   if(pos->right)
      return pos->right;
   else
      return begin();
}
list_items::iterator list_items::erase(list_items::iterator pos)
{
    Item* self = first_item ;
    Item* selfbef = self;
    
    list_items::iterator it(pos->right);

    while(self->right != pos->right) {selfbef = self; self = self->right;}

    if(self == first_item)
        first_item = pos->right;

    selfbef->right = pos->right;

    free(self);
    return it; 
}
bool list_items::isEmpty()
{
    if(begin() == end())
       return true; 
    else    
       return false;
}
void list_items::set_price(iterator it, int price)
{
  it->price = price;  
}
void list_items::set_uid(iterator it, byte* uid)
{
  it->uid = uid;
}

    /*
int main()
{
    list_items l{"item_1,item_2,item_3,item_4"};
    byte item1_uid[4] = {0x23, 0x7a, 0x4f, 0x1b};
    byte item2_uid[4]= {0x09, 0xcb, 0x8b, 0x63};
    byte item3_uid[4] = {0x13, 0x7a, 0x4f, 0x1b};
    byte item4_uid[4]= {0x29, 0xcb, 0x8b, 0x63};
    
    list_items::iterator it1 = l.begin();
    l.set_price(it1, 10);
    l.set_uid(it1, item1_uid);
    l.mark_as_bought(it1);
    it1.print();
    it1 = l.next(it1);
    l.set_price(it1, 20);
    l.set_uid(it1, item2_uid);
    l.mark_as_bought(it1);
    it1.print();
    it1 = l.next(it1);
    l.set_price(it1, 30);
    l.set_uid(it1, item3_uid);
    l.mark_as_bought(it1);
    it1.print();
    it1 = l.next(it1);
    l.set_price(it1, 40);
    l.set_uid(it1, item4_uid);
    l.mark_as_bought(it1);
    it1.print();
    //printf("\nPRICE %d -- %s", l.get_item_price(it1), *it1); 
    
    it1 = l.erase(it1);

    it1 = l.next(it1);
    it1.print();

    it1 = l.next(it1);
    it1.print();

    it1 = l.erase(it1);
    it1.print();

    it1 = l.erase(it1);
    if(l.isEmpty())
        puts("empty");
    list_items::iterator it2 = l.begin();
    //printf("%s,%s\n", *it1, *it2);
    //printf("%d", *it1);
    it2 = l.erase(it2);
    it2 = l.erase(it2);
    l.print_list();
    it2 = l.erase(it2);

    printf("%d", l.isEmpty());

    return 0;

}
    */
