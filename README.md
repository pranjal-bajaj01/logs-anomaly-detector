# System Log Anomaly Detector (C++)

## Problem Statement
Detect abnormal behavior in application logs by analyzing error rates in real time.

## Log Format
YYYY-MM-DD HH:MM:SS LEVEL SERVICE MESSAGE

Example:
2025-01-12 10:05:32 ERROR AuthService DB timeout

## Detection Logic
An anomaly is detected when:
current_error_rate > mean + 3 * standard_deviation

## Window Configuration
- Sliding window: 60 seconds
- Silence threshold: 120 seconds
- Baseline warm-up: 2 minutes

## Architecture
- Log Generator (simulates backend logs)
- Log Parser
- Sliding Window Manager
- Adaptive Baseline Detector
- Alert Engine

## Testing Strategy
Logs are generated using a controlled log generator to simulate normal traffic,
error spikes, and service silence.

## Limitations
- Single-node implementation
- File-based log ingestion
