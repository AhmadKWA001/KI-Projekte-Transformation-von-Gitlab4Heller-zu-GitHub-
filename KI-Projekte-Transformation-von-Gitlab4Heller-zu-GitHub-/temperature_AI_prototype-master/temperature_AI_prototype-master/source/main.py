from AIService import  AIService
def main():
    try:
        aiService= AIService()
        aiService.run()
    except Exception as exp:
        print(exp)
        exit()
if __name__ == "__main__":
    main()
