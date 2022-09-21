void main(void){
    __asm__("mov $0xb800, %ax");
    __asm__("mov %ax, %ds");
    int *x = (int *) 0;
    *x=0xf065f068;
    x=(int *)4;
    *x=0xf06cf06c;
    x=(int *)8;
    *x=0xf02cf06f;
    x=(int *)12;
    *x=0xf077f020;
    x=(int *)16;
    *x=0xf072f06f;
    x=(int *)20;
    *x=0xf064f06c;
    while(1){

    }
}
