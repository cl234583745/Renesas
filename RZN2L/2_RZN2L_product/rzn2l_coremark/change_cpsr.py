# --- --- stop handler ---
def stop_handler (event):
    pc = gdb.parse_and_eval('$pc');
    address = gdb.parse_and_eval('system_init');
    if pc == address:
        gdb.execute('set $cpsr = $cpsr & 0xffffffdf');
        gdb.events.stop.disconnect (stop_handler)
# --- stop handler ---

gdb.events.stop.connect (stop_handler)