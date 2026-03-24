#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>


FILE *f_battery_cp;
FILE *f_battery_st;
char buffer_cp[5];
char buffer_st[32];
char c, count = 0;
int battery = 0;
#define PLAY_HIGH_SOUND "pw-play /home/Paul/.sys_sounds/battery_full.wav"
#define PLAY_LOW_SOUND  "pw-play /home/Paul/.sys_sounds/battery_low.wav"
#define BAT_UPPER       90
#define BAT_LOWER       20
#define BAT_SLEEP       15
#define SLEEP_INTERVAL  10
const char* fp_battery_cp = "/sys/class/power_supply/BAT0/capacity";
const char* fp_battery_st = "/sys/class/power_supply/BAT0/status";


bool getStatus(char[]);
void playAudio(int);
void sys_sleep();

void debugi(int);
void debugs(char*);
void debugz(char *);

int main() {
    while (1) {
        c = count = battery = 0;
        f_battery_cp = fopen(fp_battery_cp, "r");
        f_battery_st = fopen(fp_battery_st, "r");
        if (f_battery_cp == NULL) {
            printf("Battery capicity file path is either incorrect or\nbattery does not exit.\n");
            return -1;
        }
        if (f_battery_st == NULL) {
            printf("Battery charging status file path is either incorrect or\nbattery does not exit.\n");
            return -1;
        }
        while ((c = fgetc(f_battery_cp)) != EOF ) {
            buffer_cp[count++] = c;
        }
        buffer_cp[count] = '\0';
        battery = atoi(buffer_cp);
        count = 0;
        while ((c = fgetc(f_battery_st)) != EOF ) {
            buffer_st[count++] = c;
        }
        buffer_st[count] = '\0';
        battery = 5; //TODO: Remove later
        if (getStatus(buffer_st)) {
            if (battery > BAT_UPPER) playAudio(1);
        } else {
            if (battery < BAT_SLEEP) {
                sys_sleep();
            } else {
                if (battery < BAT_LOWER)  playAudio(0);
            }
        }
        sleep(SLEEP_INTERVAL);
        fclose(f_battery_cp);
        fclose(f_battery_st);
    }
}


bool getStatus(char str[]) {
    if (!strcmp(str, "Discharging\n") || !strcmp(str, "Not charging\n") || !strcmp(str, "Unknown\n")) { return false;
    } else {
        if (!strcmp(str, "Charging\n") || !strcmp(str, "Full\n")) return true;
    }
    return false;
}

void playAudio(int status) {
    if (status) {system(PLAY_HIGH_SOUND); return;}
    else {system(PLAY_LOW_SOUND); return;}
}

void sys_sleep() {
    //system("sleep");
    printf("sleeping now");
    return;
}

void debugi(int i) {printf("%i\n", i);}
void debugs(char *i) {printf("-%s-\n", i);}
void debugz(char *s) {int c, count; c = count = 0; while ((c = s[count++])!='\0'){printf("%i\n", c);}}
