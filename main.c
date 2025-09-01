#include <stdio.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <ncurses.h>
#include "cpu/cpu.h"
#include "gpu/gpu.h"
#include "ram/ram.h"
#include "utils/find_hwmon.h"

void draw_ui() {
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
    timeout(500); // Non-blocking getch with 1-second refresh
    start_color();

    // Color pairs
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);

    while (1) {
        clear();

        // CPU
        double cpuTemp = cpu_temp(); 
        int cpuUsage = cpu_usage();
        double cpuSpeed = cpu_speed();
        double cpuMaxSpeed = cpu_max_speed();

        // GPU
        double gpuTemp = gpu_temp();
        int gpuUsage = gpu_usage();
        double vramUsed = vram_used();
        double vramTotal = vram_total();
        double vramPercent = (vramUsed / vramTotal) * 100.0;

        // RAM
        double totalRam = ram_info("MemTotal");
        double freeRam = ram_info("MemAvailable");

        // Uptime
        struct sysinfo info;
        sysinfo(&info);
        int uptime_hours = info.uptime / 3600;
        int uptime_minutes = (info.uptime % 3600) / 60;
        int uptime_seconds = info.uptime % 60;

        // Draw CPU section
        attron(A_BOLD);
        mvprintw(1, 2, "CPU Metrics");
        attroff(A_BOLD);
        mvprintw(2, 4, "Temperature: %.1f°C", cpuTemp);
        mvprintw(3, 4, "Usage: %d%%", cpuUsage);
        mvprintw(4, 4, "Speed: %.1fMHz / %.1fMHz", cpuSpeed, cpuMaxSpeed);

        // Draw GPU section
        attron(A_BOLD);
        mvprintw(6, 2, "GPU Metrics");
        attroff(A_BOLD);
        mvprintw(7, 4, "Temperature: %.1f°C", gpuTemp);
        mvprintw(8, 4, "Usage: %d%%", gpuUsage);
        mvprintw(9, 4, "VRAM: %.1fGB / %.1fGB (%.1f%%)", vramUsed, vramTotal, vramPercent);

        // Draw RAM section
        attron(A_BOLD);
        mvprintw(11, 2, "RAM Metrics");
        attroff(A_BOLD);
        mvprintw(12, 4, "Total: %.1fGB", totalRam);
        mvprintw(13, 4, "Free: %.1fGB", freeRam);

        // Draw Uptime
        attron(A_BOLD);
        mvprintw(15, 2, "System Uptime");
        attroff(A_BOLD);
        mvprintw(16, 4, "%dh %dm %ds", uptime_hours, uptime_minutes, uptime_seconds);

        // Refresh screen
        refresh();

        // Exit if user presses 'q'
        int ch = getch();
        if (ch == 'q' || ch == 'Q') break;
    }

    // End ncurses
    endwin();
}

int main() {
    draw_ui();
    return 0;
}
