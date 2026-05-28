#include "loadModels.h"

Models models;

int loadModels(){
    if (!load_model(&models.tree, "assets/tree/tree.obj")){
        printf("Failed to load tree model!\n");
        return 1;
    }
    else scale_model(&models.tree, 2.0, 2.0, 2.0); 

    if(!load_model(&models.cardboardBox, "assets/box/carboard-box.obj")) {
        printf("Failed to load cardboard box model!\n");
        return 1;
    }
    else scale_model(&models.cardboardBox, 0.2, 0.2, 0.2); 

    if(!load_model(&models.house,"assets/house/house_walls.obj")){
        printf("Failed to load cottage model!\n");
        return 1;
    }
    else scale_model(&models.house, 1.0, 1.0, 1.0); 

    if(!load_model(&models.roof,"assets/house/roof.obj")){
        printf("Failed to load cottage model!\n");
        return 1;
    }
    else scale_model(&models.roof, 1.0, 1.0, 0.85); 
    
    if(!load_model(&models.door,"assets/house/door.obj")){
        printf("Failed to load cottage model!\n");
        return 1;
    }
    else scale_model(&models.door, 0.3, 0.3, 0.3);

    if(!load_model(&models.uaz, "assets/uaz/highQualityUAZ.obj")){
        printf("Failed to load uaz model!\n");
        return 1;
    }
    else scale_model(&models.uaz, 2.0, 2.0, 2.0); 

    return 0;
}
