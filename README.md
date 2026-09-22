# Account Management System (C)

Konsol tabanlı, basit bir banka hesap yönetim sistemi. Hesap oluşturma,
listeleme, para yatırma/çekme ve hesap silme işlemlerini destekler.
Hesap verileri, program kapatılıp açıldığında kaybolmaması için
`accounts.dat` adlı binary bir dosyada saklanır.

> **Not:** Bu proje, C dilini ve dosya I/O, struct kullanımı gibi temel
> kavramları pratik yapmak amacıyla öğrenme amaçlı olarak yazılmıştır.

## Kullanılan Teknolojiler

- **Dil:** C
- **Standart kütüphaneler:** `stdio.h`, `stdlib.h`, `string.h`
- **Veri saklama:** Binary dosya (`fread`/`fwrite`)

## Özellikler

Program çalıştırıldığında aşağıdaki menü sunulur:

1. **Create New Account** — Yeni hesap oluşturma (hesap numarası otomatik üretilir)
2. **List All Accounts** — Tüm hesapları listeleme
3. **Deposit Money** — Hesaba para yatırma
4. **Withdraw Money** — Hesaptan para çekme (bakiye kontrolü yapılır)
5. **Delete Account** — Hesap silme (onay istenir)
6. **Exit** — Programdan çıkış

