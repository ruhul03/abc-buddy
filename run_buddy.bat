@echo off
title ABC Buddy - Full Platform Launcher
echo =================================================================
echo   ABC Buddy - Smart Educational Robot Platform
echo =================================================================
echo.
echo Starting Python Backend Server on Port 5000...
echo Parent Portal: http://localhost:5000/parent
echo Robot Control and AI Studio: http://localhost:5000/
echo.

start "" "http://localhost:5000/"
python server.py
pause
