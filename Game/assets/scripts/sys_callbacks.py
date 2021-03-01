import engine

def start_callback():
    engine.game.log("Start callback triggered")
    
def end_callback():
    engine.game.log("End callback triggered")

cb = engine.game.get_callback_handler()

cb.add_callback("TestStartTrigger", start_callback)
cb.add_callback("TestEndTrigger", end_callback)