#include <stdio.h>

struct Brick{ 
    int x;
    int y;
    int z;
};

int setSize(struct Brick* brick){
    int x,y,z;
    printf("Add meg x értékét:");
    if(scanf("%d",&brick->x) && &brick->x<0) return 1;
    printf("Add meg x értékét:");
    if(scanf("%d",&brick->y) && &brick->y<0) return 1;
    printf("Add meg x értékét:");
    if(scanf("%d",&brick->z) && &brick->z<0) return 1;

    return 0;
}

void calculateVolume(struct Brick* brick){

}

int main() {
    struct Brick brick;
    if(setSize(&brick) == 1) printf("Input error!");
    printf("%d\n",brick.x);
    
    return 0;
}