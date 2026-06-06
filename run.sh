source modules/gesture/.venv/bin/activate
python3 modules/gesture/main.py &
PYTHON_PID=$!


cleanup() {
    echo "Завершение работы... Убиваем python3 (PID: $PYTHON_PID)"
    kill "$PYTHON_PID" 2>/dev/null
}

trap cleanup EXIT INT TERM
./build/FuturistPlugin_artefacts/Standalone/Futurist.app/Contents/MacOS/Futurist
