from AIService import  AIService, rounding, get_abs_value
def main():
    try:
        aiService= AIService()
        aiService.run()
    except Exception as exp:
        print(exp)
        exit()
if __name__ == "__main__":
    main()
