package com.biometric.model;

import org.springframework.data.annotation.Id;
import org.springframework.data.mongodb.core.mapping.Document;

@Document(collection = "fingerprints")
public class FingerprintTemplateEntity {

    @Id
    private String name;        // username
    private byte[] template;    // encrypted ISO

    // getters and setters
    public String getName() { return name; }
    public void setName(String name) { this.name = name; }

    public byte[] getTemplate() { return template; }
    public void setTemplate(byte[] template) { this.template = template; }
}
