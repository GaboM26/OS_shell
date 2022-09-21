void main(void){
    __asm__("mov $0xb800, %ax");
    __asm__("mov %ax, %ds");
    int *x = (int *) 1992;
    *x=0xf065f068;
    x=(int *)1996;
    *x=0xf06cf06c;
    x=(int *)2000;
    *x=0xf02cf06f;
    x=(int *)2004;
    *x=0xf077f020;
    x=(int *)2008;
    *x=0xf072f06f;
    x=(int *)2012;
    *x=0xf064f06c;
    while(1){

    }
}
