// WRITE YOUR OWN
int my_str_cmp(const char *s1, const char *s2);


int main() {
    my_str_cmp("apple", "apple ");

    my_str_cmp("h", "vv");
}
// gcc -I ./include/ unit_test_strcmp.c str_cmp.c -o unit_test_str_cmp.bin
