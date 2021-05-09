/*
Copyright (C) 2021  Henrik Ullman
License: GPL Version 3
*/

#ifndef PROMETHEUS_H
#define PROMETHEUS_H


void init_prometheus(void);

void init_metric_prometheus(void);

int log_prometheus(const char* label, double value);

void close_prometheus(void);

#endif /* PROMETHEUS_H */
