import fme

def do_something():
    fme.insert("I'm called")

if __name__ == "__main__":
    fme.set_kbd(do_something)
