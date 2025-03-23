pkgname=esmu
pkgver=1.0.0
pkgrel=1
pkgdesc="A simple mathematical interpreter"
arch=('x86_64')
url="https://github.com/EchoTheDeveloper/esmu"
license=('MIT')
depends=('gcc' 'make')
source=("git+https://github.com/EchoTheDeveloper/esmu.git")
sha256sums=('SKIP')

build() {
  cd "$srcdir/$pkgname"
  make
}

package() {
  cd "$srcdir/$pkgname"
  install -Dm755 esmu "$pkgdir/usr/local/bin/esmu"
}
