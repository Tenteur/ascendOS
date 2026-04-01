#include <stdio.h>

int main() {
    char filename[256];
    char newFilename[256];
    char ch;
    int isInQuote = 0b0;
    int isAComment = 0b0;
    int ignoreNextChar = 0b0;

    FILE *file;
    FILE *newFile;

    printf("Enter the filename as .jsonc: ");
    if (scanf("%255s", filename) != 1) {
        printf("Failed to read filename.\n");
        return 1;
    }
    printf("Enter the new filename as .jsonUI: ");
    if (scanf("%255s", newFilename) != 1) {
        printf("Failed to create file.\n");
        return 1;
    }

    file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file: %s\n", filename);
        return 1;
    }

    newFile = fopen(newFilename, "w");

    while ((ch = fgetc(file)) != EOF) { // @todo: modify so it removes the empty lines.
        if (!ignoreNextChar) { // This is the second slash from the start of the comment
            if (ch == '"') isInQuote = isInQuote ^ 0b1; // OR isInQuote

            else { // This is not a quote so can be a comment
                if (ch == '/' && !isInQuote) {
                    isAComment = 1;
                    ignoreNextChar = 1;
                }
                if (ch == 0x0A) { // This is a newline so no more comment
                    isAComment = 0;
                    ignoreNextChar = 0;
                }
            }
            if (!isAComment) {
                printf("%c", ch);
                putc(ch, newFile);
            }
        } else {
            if ((ch = fgetc(file)) == EOF) break;
            ignoreNextChar = 0;
        }
    }
    // putc(0x00, newFile);
    // Closing the file

    fclose(file);
    fclose(newFile);
    printf("File closed successfully.\n");
    return 0;
}