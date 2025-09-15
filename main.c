#include <stdio.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <ncurses.h>
#include "cpu/cpu.h"
#include "gpu/gpu.h"
#include "ram/ram.h"
#include "utils/find_hwmon.h"

typedef enum { TAB_SYSTEM, TAB_PROCESSES} Tab;

void draw_system() {
    int row = 1;

    struct sysinfo info;
    sysinfo(&info);
    int hours = info.uptime / 3600;
    int minutes = (info.uptime % 3600) / 60;
    int seconds = info.uptime % 60;
    attron(A_BOLD);
    mvprintw(row++, 2, "System Uptime: %dh %dm %ds", hours, minutes, seconds);
    attroff(A_BOLD);

    row++;

    /**
     * CPU
     */
    double cpuTemp = cpu_temp(); 
    int cpuUsage = cpu_usage();
    double cpuSpeed = cpu_speed();
    double cpuMaxSpeed = cpu_max_speed();
    attron(A_BOLD);
    mvprintw(row++, 2, "CPU Metrics");
    attroff(A_BOLD);
    mvprintw(row, 4, "Temperature: ");
    if (cpuTemp < 60) attron(COLOR_PAIR(1));
    else if (cpuTemp < 80) attron(COLOR_PAIR(2));
    else attron(COLOR_PAIR(3));
    printw("%.1f°C", cpuTemp);
    attroff(COLOR_PAIR(1) | COLOR_PAIR(2) | COLOR_PAIR(3));
    row++;
    mvprintw(row, 4, "Usage: ");
    if (cpuUsage < 50) attron(COLOR_PAIR(1));
    else if (cpuUsage < 80) attron(COLOR_PAIR(2));
    else attron(COLOR_PAIR(3));
    printw("%d%%", cpuUsage);
    attroff(COLOR_PAIR(1) | COLOR_PAIR(2) | COLOR_PAIR(3));
    row++;

    mvprintw(row++, 4, "Speed: %.1fMHz / %.1fMHz", cpuSpeed, cpuMaxSpeed);

    row++;

    /**
     * GPU
     */
    double gpuTemp = gpu_temp();
    int gpuUsage = gpu_usage();
    double vramUsed = vram_used();
    double vramTotal = vram_total();
    double vramPercent = (vramUsed / vramTotal) * 100.0;
    attron(A_BOLD);
    mvprintw(row++, 2, "GPU Metrics");
    attroff(A_BOLD);
    mvprintw(row, 4, "Temperature: ");
    if (gpuTemp < 65) attron(COLOR_PAIR(1));
    else if (gpuTemp < 85) attron(COLOR_PAIR(2));
    else attron(COLOR_PAIR(3));
    printw("%.1f°C", gpuTemp);
    attroff(COLOR_PAIR(1) | COLOR_PAIR(2) | COLOR_PAIR(3));
    row++;
    mvprintw(row, 4, "Usage: ");
    if (gpuUsage < 50) attron(COLOR_PAIR(1));
    else if (gpuUsage < 80) attron(COLOR_PAIR(2));
    else attron(COLOR_PAIR(3));
    printw("%d%%", gpuUsage);
    attroff(COLOR_PAIR(1) | COLOR_PAIR(2) | COLOR_PAIR(3));
    
    row++;

    /**
     * RAM
     */
    mvprintw(row++, 4, "VRAM: %.1fGB / %.1fGB (%.1f%%)", vramUsed, vramTotal, vramPercent);
    double totalRam = ram_info("MemTotal");
    double freeRam = ram_info("MemAvailable");

    row++;

    attron(A_BOLD);
    mvprintw(row++, 2, "RAM Metrics");
    attroff(A_BOLD);
    mvprintw(row++, 4, "Free: %.1fGB / %.1fGB", freeRam, totalRam);
    mvprintw(row++, 4, "Used: %.1fGB / %.1fGB", totalRam - freeRam, totalRam);
}


void draw_processes() {
    attron(A_BOLD);
    mvprintw(1, 2, "Coming soon");
    attroff(A_BOLD);
}

void draw_bar(int row, int col, int width, double percent) {
    int filled = (int)((percent / 100.0) * width);
    for (int i = 0; i < width; i++) {
        if (i < filled) attron(COLOR_PAIR(2)); // filled part
        else attron(COLOR_PAIR(1));            // empty part
        mvprintw(row, col + i, "█");
        attroff(COLOR_PAIR(1) | COLOR_PAIR(2));
    }
}


void draw_ui() {
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
    timeout(900); 
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);

    Tab current_tab = TAB_SYSTEM;

    while (1) {
        clear();

        // Draw tab header
        attron(A_REVERSE);
        mvprintw(0, 2, " [S] System ");
        mvprintw(0, 15, " [P] Processes ");
        attroff(A_REVERSE);

        switch (current_tab) {
            case TAB_SYSTEM: attron(COLOR_PAIR(4)); mvprintw(0, 2, " [S] System "); attroff(COLOR_PAIR(1)); break;
            case TAB_PROCESSES: attron(COLOR_PAIR(4)); mvprintw(0, 15, " [P] Processes "); attroff(COLOR_PAIR(1)); break;
        }

        switch (current_tab) {
            case TAB_SYSTEM: draw_system(); break;
            case TAB_PROCESSES: draw_processes(); break;
        }

        refresh();

        int ch = getch();
        if (ch == 'q' || ch == 'Q') break;
        else if (ch == 's' || ch == 'S') current_tab = TAB_SYSTEM;
        else if (ch == 'p' || ch == 'P') current_tab = TAB_PROCESSES;
    }

    endwin();
}

int main() {
    draw_ui();
    return 0;
}
