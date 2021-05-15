
# First, defining the Submission class

class Submission(): 
    """A Submission is a part of the overall Mission."""
    def __init__(self,name,ddl):
        self.__name__ = name
        self.__ddl__ = ddl

# Define the initial overall Mission list
Mission = 