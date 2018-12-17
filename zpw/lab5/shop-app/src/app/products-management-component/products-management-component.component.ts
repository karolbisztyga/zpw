import { Component, OnInit } from '@angular/core';
import { AuthServiceService } from '../auth-service.service';
import { Router } from '@angular/router';
import { ProductServiceService } from '../product-service.service';
import { Product } from '../objects/product';
import { DbserviceService } from '../dbservice.service';

@Component({
  selector: 'app-products-management-component',
  templateUrl: './products-management-component.component.html',
  styleUrls: ['./products-management-component.component.scss']
})
export class ProductsManagementComponentComponent implements OnInit {

  public newProduct = new Product()
  public products: Product[]

  constructor(
    private authService:AuthServiceService,
    private productsSrvice:ProductServiceService,
    private router: Router,
    private dbservice: DbserviceService) { }

  ngOnInit() {
    if (!this.authService.isAdmin(this.authService.getUser().email)) {
      this.router.navigateByUrl('/admin/panel/dummy')
    }
    this.products = this.productsSrvice.products
  }


  update(id, key, val) {
    this.productsSrvice.updateProduct(id, key, val)
  }

  save(id) {
    for (let i in this.productsSrvice.products) {
      let p = this.productsSrvice.products[i]
      if (p.id == id) {
        this.productsSrvice.saveProduct(p.id, function() {
          window.location.reload()
        })
        return
      }
    }
  }

  nupdate(key, val) {
    switch(key) {
      case "name": {
        this.newProduct.name = val
        break;
      }
      case "price": {
        this.newProduct.price_for_one = parseFloat(val)
        break;
      }
      case "quantity": {
        this.newProduct.quantity = parseInt(val)
        break;
      }
      case "imgurl": {
        this.newProduct.img_url = val
        break;
      }
      case "description": {
        this.newProduct.description = val
        break;
      }
      case "categories": {
        this.newProduct.categories = (val+"").split(',')
        break;
      }
    }
  }

  nsave() {
    let p = this.newProduct
    if (p.name.length == 0) {
      console.log('invalid name: ' + p.name)
      return
    }
    if (p.description.length == 0) {
      console.log('invalid description')
      return
    }
    if (p.categories.length == 0) {
      console.log('invalid categories')
      return
    }
    if (p.price_for_one == 0) {
      console.log('invalid price_for_one')
      return
    }
    if (p.img_url.length == 0) {
      console.log('invalid img_url')
      return
    }
    if (p.quantity == 0) {
      console.log('invalid quantity')
      return
    }

    console.log('nsaving product ')
    console.log(this.newProduct)
    this.productsSrvice.addProduct(this.newProduct, function() {
      window.location.reload()
    })
  }

  changeDb(db) {
    if (this.dbservice.changeDatabase(db)) {
      window.location.reload()
    }
  }

}
