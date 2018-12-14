import { Component, OnInit } from '@angular/core';
import { AngularFireDatabase, AngularFireList } from 'angularfire2/database';
import { Order } from '../objects/orders';
import { OrderServiceService } from '../order-service.service';
import { ProductServiceService } from '../product-service.service';
import { Router } from '@angular/router';

@Component({
  selector: 'app-orders-management-component',
  templateUrl: './orders-management-component.component.html',
  styleUrls: ['./orders-management-component.component.scss']
})
export class OrdersManagementComponentComponent implements OnInit {
  public data: AngularFireList<any[]>

  constructor(private db: AngularFireDatabase,
    public orderService: OrderServiceService,
    private productsSrvice:ProductServiceService,
    private router: Router) { }

  ngOnInit() {
    this.productsSrvice.getProducts()
  }

  finalizeOrder(orderId) {
    let products = this.orderService.getProductsOfOrder(orderId)
    let ok = this.productsSrvice.performOrder(products)
    if (ok) {
      this.orderService.finalizeOrder(orderId)
    }
    window.location.reload();
  }

  isFinalized(orderId) {
    return this.orderService.isOrderFinalized(orderId)
  }

}
