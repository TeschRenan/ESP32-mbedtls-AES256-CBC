#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "mbedtls/aes.h"

mbedtls_aes_context aes;

unsigned char key[32] = "Is this key secure? maybe yes!!!";

unsigned char iv[16] = "encryption-ESP32";
unsigned char iv1[16] = "encryption-ESP32";

unsigned char content[64] = {0};

void decrypt(uint8_t *data, uint16_t size)
{
    uint8_t decrypt_input[16] = {0};
    uint8_t decrypt_output[16] = {0};

    for (uint16_t j = 0; j < size; j += 16)
    {
        for (int8_t i = 0; i < 16; i++)
        {
            decrypt_input[i] = (j+i > size) ? 0 : data[j+i];
        }

        mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, 16, iv, decrypt_input, decrypt_output);

        for (int8_t i = 0; i < 16; i++)
        {
            data[j+i] = decrypt_output[i];
        }
    }
}

void encrypt(uint8_t *data, uint16_t size)
{
    uint8_t encrypt_input[16] = {0};
    uint8_t encrypt_output[16] = {0};

    for (uint16_t j = 0; j < size; j += 16)
    {
        for (int8_t i = 0; i < 16; i++)
        {
            encrypt_input[i] = (j+i > size) ? 0 : data[j+i];
        }

        mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, 16, iv1, encrypt_input, encrypt_output);

        for (int8_t i = 0; i < 16; i++)
        {
            data[j+i] = encrypt_output[i];
        }
    }
}

void app_main()
{
    sprintf((char*)content, "%s","Testing the Encrypt and Decrypt using AES256-CBC");

    mbedtls_aes_init(&aes);

    mbedtls_aes_setkey_enc(&aes,key, 256);
    
    encrypt(content,64);
    
    ESP_LOG_BUFFER_HEX("cbc", content, 64);
    ESP_LOGI("cbc", "%s", content);

    decrypt(content,64);

    ESP_LOG_BUFFER_HEX("cbc", content, 64);
    ESP_LOGI("cbc", "%s", content);
    
    mbedtls_aes_free(&aes);
}
